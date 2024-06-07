#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createMainWindow();
    createMenuBar();
    createStatusBar();
    createToolBar();
    createCentralWidget();

    translations = new TranslationsModel(this);

    createPageView();
    createEditPane();

    QHBoxLayout *centralLayout = new QHBoxLayout(mCentralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(0);
    centralLayout->addWidget(pageView);
    centralLayout->addLayout(editPane);
}

MainWindow::~MainWindow() {}

void MainWindow::createMainWindow()
{
    setObjectName("MainWindow");
    setWindowTitle("comiCAT");
    resize(WINDOW_INIT_WIDTH, WINDOW_INTI_HEIGHT);

    mOpen = new QAction("Open");
    mOpen->setShortcut(QKeySequence::Open);
    mOpen->setStatusTip("Open an existing file");
    connect(mOpen, &QAction::triggered, this, &MainWindow::openFile);

    mTBoxSelect = new QAction(QIcon(":/resources/icons/select.svg"), "Select");
    mTBoxDirectSelect = new QAction(QIcon(
                                        ":/resources/icons/select-direct.svg"),
                                    "Direct select");
    mTBoxAdd = new QAction(QIcon(":/resources/icons/add.svg"), "Add");
    mTBoxMerge = new QAction(QIcon(":/resources/icons/merge.svg"), "Merge");
    mTBoxSplit = new QAction(QIcon(":/resources/icons/split.svg"), "Split");
    mTBoxRemove = new QAction(QIcon(":/resources/icons/erase.svg"), "Remove");
    mCanvasMove = new QAction(QIcon(":/resources/icons/hand.svg"), "Hand");
    mCanvasZoom = new QAction(QIcon(":/resources/icons/zoom.svg"), "Zoom");
    mSettings = new QAction(QIcon(":/resources/icons/settings.svg"), "Settings");

    QMetaObject::connectSlotsByName(this);
}

void MainWindow::createCentralWidget()
{
    mCentralWidget = new QWidget(this);
    mCentralWidget->setObjectName("CentralWidget");
    setCentralWidget(mCentralWidget);
    mCentralWidget->setStyleSheet("QWidget#CentralWidget {"
                                  "background-color: white;"
                                  "} QWidget {"
                                  "color: #000000;"
                                  "}");
}

void MainWindow::createMenuBar()
{
    mMenuBar = new QMenuBar(mCentralWidget);
    mMenuBar->setObjectName("MenuBar");

    mFileMenu = mMenuBar->addMenu(tr("File", "MenuBar"));
    mFileMenu->addAction(tr("New", "MenuBar_File"));
    mFileMenu->addAction(mOpen);
    mFileMenu->addAction(tr("Save", "MenuBar_File"));
    mFileMenu->addAction(tr("Save As", "MenuBar_File"));
    mExportMenu = mFileMenu->addMenu(tr("Export", "MenuBar_File"));
    mExportMenu->addAction(tr("Export as .txt", "MenuBar_File"));
    mFileMenu->addAction(tr("Close", "MenuBar_File"));
    mFileMenu->addAction(tr("Quit", "MenuBar_File"));

    setMenuBar(mMenuBar);
}

void MainWindow::createStatusBar()
{
    mStatusBar = new QStatusBar(mCentralWidget);
    mStatusBar->setObjectName("StatusBar");
    setStatusBar(mStatusBar);
    mStatusBar->setStyleSheet("QStatusBar#StatusBar {"
                              "outline: none;"
                              "border-style: solid;"
                              "border-color: #e0e0e0;"
                              "border-width: 1 0 0 0;"
                              "background-color: white;"
                              "color: #828282;"
                              "font-size: 11px;"
                              "}");
    mStatusBar->showMessage("Ready!", 3000);
}

void MainWindow::createToolBar()
{
    mToolBar = new QToolBar(mCentralWidget);
    mToolBar->setObjectName("ToolBar");
    mToolBar->setFloatable(false);
    mToolBar->setMovable(false);
    mToolBar->setAllowedAreas(Qt::LeftToolBarArea);
    mToolBar->setOrientation(Qt::Vertical);
    mToolBar->setIconSize(QSize(16, 16));
    mToolBar->setStyleSheet("QToolBar#ToolBar {"
                            "outline: none;"
                            "border-style: solid;"
                            "border-color: #e0e0e0;"
                            "border-width: 0 1 0 0;"
                            "padding: 8 4; background-color: white;"
                            "} QToolBar::separator {"
                            "background: #e0e0e0; height:1px; margin: 6;"
                            "} QToolButton {"
                            "width: 28; background-color: white;"
                            "padding: 6 4; border-radius: 4;"
                            "}");

    mToolBar->addAction(mTBoxSelect);
    mToolBar->addAction(mTBoxDirectSelect);
    mToolBar->addSeparator();
    mToolBar->addAction(mTBoxAdd);
    mToolBar->addAction(mTBoxMerge);
    mToolBar->addAction(mTBoxSplit);
    mToolBar->addAction(mTBoxRemove);
    mToolBar->addSeparator();
    mToolBar->addAction(mCanvasMove);
    mToolBar->addAction(mCanvasZoom);
    mToolBar->addSeparator();
    mToolBar->addAction(mSettings);

    addToolBar(Qt::LeftToolBarArea, mToolBar);
}

void MainWindow::openFile()
{
    QString fn
        = QFileDialog::getOpenFileName(this,
                                       tr("Open Image"),
                                       "",
                                       tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fn.isEmpty()) {
        filename = fn;
        pageView->loadImg(filename, translations);
    }
}

void MainWindow::createPageView()
{
    pageView = new PageView(this);
    pageView->setStyleSheet("QGraphicsView {"
                            "border: none; outline: none;"
                            "background-color: white;"
                            "}");
}

void MainWindow::createEditPane()
{
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    translationList = new TranslationList(this, translations);
    sizePolicy.setVerticalStretch(4);
    translationList->setSizePolicy(sizePolicy);

    translationEditor = new TranslationEditor(this);
    sizePolicy.setVerticalStretch(3);
    translationEditor->setSizePolicy(sizePolicy);

    editPane = new QVBoxLayout();
    editPane->setContentsMargins(16, 16, 16, 16);
    editPane->setSpacing(16);
    editPane->addWidget(translationList);
    editPane->addWidget(translationEditor);
}
