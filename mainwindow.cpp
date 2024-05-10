#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupMainWindow();
    setupCentralWidget();
    setupMenuBar();
    setupStatusBar();
    setupToolBar();
}

MainWindow::~MainWindow() {}

void MainWindow::setupMainWindow()
{
    setObjectName("MainWindow");
    setWindowTitle("comiCAT");
    resize(WINDOW_INIT_WIDTH, WINDOW_INTI_HEIGHT);

    m_TBoxSelect = new QAction(QIcon(":/resources/icons/select.svg"), "Select");
    m_TBoxDirectSelect = new QAction(QIcon(":/resources/icons/select-direct.svg"), "Direct select");
    m_TBoxMerge = new QAction(QIcon(":/resources/icons/merge.svg"), "Merge");
    m_TBoxSplit = new QAction(QIcon(":/resources/icons/split.svg"), "Split");
    m_TBoxRemove = new QAction(QIcon(":/resources/icons/erase.svg"), "Remove");
    m_CanvasMove = new QAction(QIcon(":/resources/icons/hand.svg"), "Hand");
    m_CanvasZoom = new QAction(QIcon(":/resources/icons/zoom.svg"), "Zoom");

    QMetaObject::connectSlotsByName(this);
}

void MainWindow::setupCentralWidget() {
    m_CentralWidget = new QWidget(this);
    m_CentralWidget->setObjectName("CentralWidget");
    setCentralWidget(m_CentralWidget);
}

void MainWindow::setupMenuBar() {
    m_MenuBar = new QMenuBar(m_CentralWidget);
    m_MenuBar->setObjectName("MenuBar");
    m_MenuBar->setGeometry(QRect(0, 0, WINDOW_INIT_WIDTH, 24));

    m_FileMenu = m_MenuBar->addMenu(tr("File", "MenuBar"));
    m_FileMenu->addAction(tr("New", "MenuBar_File"));
    m_FileMenu->addAction(tr("Open", "MenuBar_File"));
    m_FileMenu->addAction(tr("Save", "MenuBar_File"));
    m_FileMenu->addAction(tr("Save As", "MenuBar_File"));
    m_ExportMenu = m_FileMenu->addMenu(tr("Export", "MenuBar_File"));
    m_ExportMenu->addAction(tr("Export as .txt", "MenuBar_File"));
    m_FileMenu->addAction(tr("Close", "MenuBar_File"));
    m_FileMenu->addAction(tr("Quit", "MenuBar_File"));

    setMenuBar(m_MenuBar);
}

void MainWindow::setupStatusBar() {
    m_StatusBar = new QStatusBar(m_CentralWidget);
    m_StatusBar->setObjectName("StatusBar");
    setStatusBar(m_StatusBar);
}

void MainWindow::setupToolBar() {
    m_ToolBar = new QToolBar(m_CentralWidget);
    m_ToolBar->setFloatable(false);
    m_ToolBar->setMovable(false);
    m_ToolBar->setAllowedAreas(Qt::LeftToolBarArea);
    m_ToolBar->setOrientation(Qt::Vertical);
    m_ToolBar->setIconSize(QSize(16, 16));

    m_ToolBar->addAction(m_TBoxSelect);
    m_ToolBar->addAction(m_TBoxDirectSelect);
    m_ToolBar->addSeparator();
    m_ToolBar->addAction(m_TBoxMerge);
    m_ToolBar->addAction(m_TBoxSplit);
    m_ToolBar->addAction(m_TBoxRemove);
    m_ToolBar->addSeparator();
    m_ToolBar->addAction(m_CanvasMove);
    m_ToolBar->addAction(m_CanvasZoom);

    addToolBar(Qt::LeftToolBarArea, m_ToolBar);
}
