#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QActionGroup>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QSizePolicy>
#include <QStatusBar>
#include <QToolBar>
#include <QWidget>

#include "pageview.h"
#include "translationeditor.h"
#include "translationlist.h"
#include "translationsmodel.h"

static const int WINDOW_INIT_WIDTH = 1000;
static const int WINDOW_INTI_HEIGHT = 600;
static const int ICON_SIZE = 16;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createActions();
    void createCentralWidget();
    void createMenuBar();
    void createStatusBar();
    void createToolBar();
    void createPageView();
    void createEditPane();

public slots:
    void openFile();
    void onCanvasZoomChanged(qreal percent);
    void onCanvasActionChanged();

public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *toolBar;
    PageView *pageView;
    QVBoxLayout *editPane;

    TranslationsModel *translations;
    TranslationList *translationList;
    TranslationEditor *translationEditor;

    QLabel *fileNameLabel;
    QLabel *pageLabel;
    QLabel *zoomLabel;
    QLabel *finishedLabel;

    QAction *actionOpen;
    QAction *actionSelect;
    QAction *actionDirectSelect;
    QAction *actionAdd;
    QAction *actionMerge;
    QAction *actionSplit;
    QAction *actionRemove;
    QAction *actionMove;
    QAction *actionZoom;
    QAction *actionOpenSettings;
    QActionGroup *canvasActions;

    QMenu *fileMenu;
    QMenu *exportMenu;
    QMenu *editMenu;
    QMenu *viewMenu;

private:
    bool isFileOpened = false;
    QAction *currCanvasAction = nullptr;
};
#endif // MAINWINDOW_H
