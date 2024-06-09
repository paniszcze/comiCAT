#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QCheckBox>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QFrame>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QPixmap>
#include <QProgressBar>
#include <QRadioButton>
#include <QRect>
#include <QSizePolicy>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QToolBar>
#include <QWidget>

#include "pageview.h"
#include "translationeditor.h"
#include "translationlist.h"
#include "translationsmodel.h"

const int WINDOW_INIT_WIDTH = 1000;
const int WINDOW_INTI_HEIGHT = 600;
const int ICON_SIZE = 16;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // main components
    QWidget *mCentralWidget;
    QMenuBar *mMenuBar;
    QToolBar *mToolBar;
    PageView *pageView;
    TranslationList *translationList;
    TranslationEditor *translationEditor;

    // menus
    QMenu *mFileMenu;
    QMenu *mExportMenu;
    QMenu *mEditMenu;
    QMenu *mViewMenu;

    // status bar labels
    QLabel *fileNameLabel;
    QLabel *pageLabel;
    QLabel *zoomLabel;
    QLabel *finishedLabel;

    // actions
    QAction *mOpen;
    QAction *mTBoxSelect;
    QAction *mTBoxDirectSelect;
    QAction *mTBoxAdd;
    QAction *mTBoxMerge;
    QAction *mTBoxSplit;
    QAction *mTBoxRemove;
    QAction *mCanvasMove;
    QAction *mCanvasZoom;
    QAction *mSettings;

    QVBoxLayout *editPane;

    TranslationsModel *translations;

    void createMainWindow();
    void createCentralWidget();
    void createMenuBar();
    void createStatusBar();
    void createToolBar();
    void createPageView();
    void createEditPane();

    void newFile();
    void openFile();
    void saveFile();
    void saveFile(const QString& path, const bool setPath = true);
    void saveFileAs(const bool setPath = true);
    void exportTXT();
    bool closeProject();

public slots:
    void fileOpened(QString filePath);
    void scaled(qreal percent);

private:
    bool isFileOpened{false};
};
#endif // MAINWINDOW_H
