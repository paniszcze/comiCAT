#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QDebug>
#include <QFileDialog>
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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QWidget *mCentralWidget;
    QMenuBar *mMenuBar;
    QMenu *mFileMenu;
    QMenu *mExportMenu;
    QMenu *mEditMenu;
    QMenu *mViewMenu;
    QStatusBar *mStatusBar;
    QToolBar *mToolBar;

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

    PageView *pageView;
    TranslationList *translationList;
    TranslationEditor *translationEditor;

    void createMainWindow();
    void createCentralWidget();
    void createMenuBar();
    void createStatusBar();
    void createToolBar();

    void createPageView();
    void createEditPane();

    void newFile();
    void saveFile();
    void saveFile(const QString& path, const bool setPath = true);
    void saveFileAs(const bool setPath = true);
    void exportTXT();
    bool closeProject();

    void setFilename(const QString &fn);
    QString getFilename() const { return filename; }

private slots:
    void openFile();

private:
    QString filename;
};
#endif // MAINWINDOW_H
