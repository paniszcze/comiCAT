#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QActionGroup>
#include <QCoreApplication>
#include <QDir>
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
#include "reader.h"
#include "translationeditor.h"
#include "translationsmodel.h"

static const int WINDOW_INIT_WIDTH = 1000;
static const int WINDOW_INTI_HEIGHT = 600;
static const int ICON_SIZE = 16;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createActions();
    void createMenuBar();
    void createStatusBar();
    void updateStatusBarInfo(QString fileName = "");
    void createToolBar();

public slots:
    void openFile();
    void closeFile();
    void onCanvasZoomChanged(qreal percent);
    void onCanvasActionChanged();
    void onItemNeedsUpdate(QModelIndex itemIndex, QString updatedText);

public:
    QMenuBar *menuBar;
    QToolBar *toolBar;

    TranslationsModel *translations;
    TranslationEditor *translationEditor;

    Reader *reader;

    QLabel *fileNameLabel;
    QLabel *pageLabel;
    QLabel *zoomLabel;
    QLabel *finishedLabel;

    QAction *actionOpen;
    QAction *actionClose;
    QAction *actionQuit;

    QAction *actionFitInWindow;
    QAction *actionActualSize;
    QAction *actionZoomIn;
    QAction *actionZoomOut;

    QActionGroup *canvasActions;
    QAction *actionSelect;
    QAction *actionDirectSelect;
    QAction *actionAdd;
    QAction *actionMerge;
    QAction *actionSplit;
    QAction *actionRemove;
    QAction *actionMove;
    QAction *actionZoom;
    QAction *actionOpenSettings;

    QMenu *fileMenu;
    QMenu *exportMenu;
    QMenu *editMenu;
    QMenu *viewMenu;

private:
    Ui::MainWindow *ui;
    bool isFileOpened;
    QString currFilePath;
    QDir lastFileDialogDir;
    QAction *currCanvasAction;
};
#endif // MAINWINDOW_H
