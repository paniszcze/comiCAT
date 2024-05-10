#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QWidget>
#include <QAction>
#include <QIcon>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <leptonica/pix_internal.h>

#define WINDOW_INIT_WIDTH  900
#define WINDOW_INTI_HEIGHT 600

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QWidget    *m_CentralWidget;
    QMenuBar   *m_MenuBar;
    QMenu      *m_FileMenu;
    QMenu      *m_ExportMenu;
    QMenu      *m_EditMenu;
    QMenu      *m_ViewMenu;
    QStatusBar *m_StatusBar;
    QToolBar   *m_ToolBar;

    QAction *m_TBoxSelect;
    QAction *m_TBoxDirectSelect;
    QAction *m_TBoxMerge;
    QAction *m_TBoxSplit;
    QAction *m_TBoxRemove;
    QAction *m_CanvasMove;
    QAction *m_CanvasZoom;

    void setupMainWindow();
    void setupCentralWidget();
    void setupMenuBar();
    void setupStatusBar();
    void setupToolBar();

    void newFile();
    void openFile();
    void openFile(const QString& openPath);
    void saveFile();
    void saveFile(const QString& path, const bool setPath = true);
    void saveFileAs(const bool setPath = true);
    void exportTXT();
    bool closeProject();

};
#endif // MAINWINDOW_H
