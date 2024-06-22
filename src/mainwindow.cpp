#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createMainWindow();
    createMenuBar();
    createStatusBar();
    createToolBar();
    createCentralWidget();
    createPageView();
    createEditPane();

    QHBoxLayout *centralLayout = new QHBoxLayout(mCentralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(0);
    centralLayout->addWidget(pageView);
    centralLayout->addLayout(editPane);

    connect(pageView, &PageView::fileOpened, this, &MainWindow::fileOpened);
    connect(pageView, &PageView::scaled, this, &MainWindow::scaled);
    connect(translations,
            &TranslationsModel::rowsMoved,
            translationList->translationList,
            &TranslationsView::onRowsMoved);
    connect(translations,
            &TranslationsModel::rowsRemoved,
            translationList->translationList,
            &TranslationsView::onRowsRemoved);
    connect(translations,
            &TranslationsModel::rowsInserted,
            translationList->translationList,
            &TranslationsView::onRowsInserted);
}

MainWindow::~MainWindow() {}

void MainWindow::createMainWindow()
{
    setObjectName("MainWindow");
    setWindowTitle("comiCAT");
    resize(WINDOW_INIT_WIDTH, WINDOW_INTI_HEIGHT);

    translations = new TranslationsModel(this);

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
    mCentralWidget->setStyleSheet(
        "QWidget#CentralWidget {background-color: white;}");
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

    // TODO:
    // View >
    //      Zoom In, Zoom Out, Fit on Screen, Actual Size
    //      Show Status Bar
    //      Show Translation Rectangles

    setMenuBar(mMenuBar);
}

void MainWindow::createStatusBar()
{
    fileNameLabel = new QLabel("");
    pageLabel = new QLabel("Page 1 of 1");
    zoomLabel = new QLabel("Zoom: 0%");
    finishedLabel = new QLabel("Finished: 0%");

    auto separator = []() { return new QLabel("|"); };

    QWidget *projectInfo = new QWidget();
    QHBoxLayout *infoLayout = new QHBoxLayout(projectInfo);
    infoLayout->setContentsMargins(10, 0, 0, 0);
    infoLayout->addWidget(pageLabel);
    infoLayout->addWidget(separator());
    infoLayout->addWidget(zoomLabel);
    infoLayout->addWidget(separator());
    infoLayout->addWidget(finishedLabel);

    statusBar()->addWidget(fileNameLabel);
    statusBar()->addPermanentWidget(projectInfo);
    statusBar()->setSizeGripEnabled(false);
    statusBar()->setContentsMargins(6, 0, 6, 0);
    statusBar()->setStyleSheet(
        "QStatusBar {background-color: white; min-height: 28px;"
        "border-top: 1px solid #e0e0e0; color: #828282; font-size: 11px;}"
        "QLabel {color: #828282; font-size: 11px;}");

    if (!isFileOpened)
    {
        fileNameLabel->hide();
        projectInfo->hide();
    }
}

void MainWindow::createToolBar()
{
    mToolBar = new QToolBar(mCentralWidget);
    mToolBar->setObjectName("ToolBar");
    mToolBar->setFloatable(false);
    mToolBar->setMovable(false);
    mToolBar->setAllowedAreas(Qt::LeftToolBarArea);
    mToolBar->setOrientation(Qt::Vertical);
    mToolBar->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    mToolBar->setStyleSheet(
        "QToolBar#ToolBar {padding: 8 4; background-color: white;"
        "outline: none; border-right: 1px solid #e0e0e0;}"
        "QToolBar::separator {background: #e0e0e0; height:1px; margin: 6;}"
        "QToolButton {width: 28; background-color: white;"
        "padding: 6 4; border-radius: 4;}");

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
    QString filePath
        = QFileDialog::getOpenFileName(this,
                                       tr("Open Image"),
                                       "",
                                       tr("Image Files (*.png *.jpg *.bmp)"));
    if (!filePath.isEmpty()) { pageView->loadImg(filePath, translations); }
}

void MainWindow::createPageView()
{
    pageView = new PageView(this);
    pageView->setStyleSheet("QGraphicsView {border: none; outline: none;"
                            "background-color: white;}");
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

void MainWindow::fileOpened(QString filePath)
{
    isFileOpened = true;

    auto labels = statusBar()->findChildren<QWidget *>(
        Qt::FindDirectChildrenOnly);
    for (auto label : labels) { label->show(); }

    QFileInfo fileInfo(filePath);
    fileNameLabel->setText(fileInfo.fileName());
}

void MainWindow::scaled(qreal percent)
{
    QString text = "Zoom: " + QString::number(percent, 'd', 0) + "%";
    zoomLabel->setText(text);
}
