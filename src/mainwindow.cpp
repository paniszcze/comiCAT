#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    isFileOpened(false),
    currFilePath(""),
    lastFileDialogDir(QDir().home()),
    currCanvasAction(nullptr)
{
    setWindowTitle("comiCAT");
    resize(WINDOW_INIT_WIDTH, WINDOW_INTI_HEIGHT);

    createCentralWidget();
    createPageView();
    createEditPane();
    createActions();
    createMenuBar();
    createStatusBar();
    createToolBar();

    QHBoxLayout *centralLayout = new QHBoxLayout(centralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(0);
    centralLayout->addWidget(pageView);
    centralLayout->addLayout(editPane);
}

MainWindow::~MainWindow() {}

void MainWindow::createCentralWidget()
{
    centralWidget = new QWidget(this);
    centralWidget->setObjectName("CentralWidget");
    setCentralWidget(centralWidget);
    centralWidget->setStyleSheet(
        "QWidget#CentralWidget {background-color: white;}"
        "QScrollBar:vertical {background: transparent; border: none;"
        "width: 10px; margin: 4px 0px 4px 4px;}"
        "QScrollBar::handle:vertical"
        "{min-height: 40px; border-radius: 3px; background: #cfcfcf;}"
        "QScrollBar::handle:vertical:hover {background: #cfcfcf;}"
        "QScrollBar::handle:vertical:pressed {background: #e0e0e0;}"
        "QScrollBar::sub-line:vertical, QScrollBar::add-line:vertical,"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical"
        "{height: 0px; background: transparent;}"
        "QScrollBar:horizontal {background: transparent; border: none;"
        "height: 14px; margin: 4px 4px 4px 4px;}"
        "QScrollBar::handle:horizontal"
        "{min-width: 40px; border-radius: 3px; background: #cfcfcf;}"
        "QScrollBar::handle:horizontal:hover {background: #cfcfcf;}"
        "QScrollBar::handle:horizontal:pressed {background: #e0e0e0;}"
        "QScrollBar::sub-line:horizontal, QScrollBar::add-line:horizontal,"
        "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal"
        "{width: 0px; background: transparent;}"
        "QAbstractScrollArea::corner {background-color: white; border: none;}");

    translations = new TranslationsModel(centralWidget);
}

void MainWindow::createMenuBar()
{
    menuBar = new QMenuBar(centralWidget);
    menuBar->setObjectName("MenuBar");

    fileMenu = menuBar->addMenu(tr("File", "MenuBar"));
    fileMenu->addAction(tr("New", "MenuBar_File"));
    fileMenu->addAction(actionOpen);
    fileMenu->addAction(tr("Save", "MenuBar_File"));
    fileMenu->addAction(tr("Save As", "MenuBar_File"));
    exportMenu = fileMenu->addMenu(tr("Export", "MenuBar_File"));
    exportMenu->addAction(tr("Export as .txt", "MenuBar_File"));
    fileMenu->addAction(actionClose);
    fileMenu->addAction(actionQuit);
    fileMenu->addSeparator();

    editMenu = menuBar->addMenu(tr("Edit", "MenuBar"));
    editMenu->addAction(tr("Undo", "MenuBar_Edit"));
    editMenu->addAction(tr("Redo", "MenuBar_Edit"));
    editMenu->addSeparator();

    viewMenu = menuBar->addMenu(tr("View", "MenuBar"));
    viewMenu->addAction(actionFitInWindow);
    viewMenu->addAction(actionActualSize);
    viewMenu->addAction(actionZoomIn);
    viewMenu->addAction(actionZoomOut);
    viewMenu->addSeparator();
    viewMenu->addAction(tr("Show Status Bar", "MenuBar_View"));
    viewMenu->addAction(tr("Show Translation Rectangles", "MenuBar_View"));
    viewMenu->addSeparator();

    setMenuBar(menuBar);
}

void MainWindow::createStatusBar()
{
    fileNameLabel = new QLabel("");
    pageLabel = new QLabel("Page 1 of 1");
    zoomLabel = new QLabel("Zoom: 100%");
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
        "border-top: 1px solid #e0e0e0;}"
        "QStatusBar::item {border: none; outline: none;}"
        "QLabel {border: none; outline: none; color: #828282;"
        "font-size: 11px; font-weight: 300;}");

    if (!isFileOpened)
    {
        fileNameLabel->hide();
        projectInfo->hide();
    }
}

void MainWindow::createToolBar()
{
    toolBar = new QToolBar(centralWidget);
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    toolBar->setAllowedAreas(Qt::LeftToolBarArea);
    toolBar->setOrientation(Qt::Vertical);
    toolBar->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    toolBar->setStyleSheet(
        "QToolBar {padding: 4px; spacing: 4px; background-color: white;"
        "outline: none; border-right: 1px solid #e0e0e0;}"
        "QToolBar::separator {height: 1px; margin: 4px; background: #e0e0e0;}"
        "QToolButton {width: 28px; padding: 6px 4px; border-radius: 4;"
        "background-color: white;}"
        "QToolButton:checked, QToolButton:pressed[checkable=\"true\"]"
        "{background-color: #e0e0e0;}"
        "QToolButton:hover:!pressed:!checked[checkable=\"true\"]"
        "{background-color: #f2f2f2;}");

    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    toolBar->addAction(actionSelect);
    toolBar->addAction(actionDirectSelect);
    toolBar->addSeparator();
    toolBar->addAction(actionAdd);
    toolBar->addAction(actionMerge);
    toolBar->addAction(actionSplit);
    toolBar->addAction(actionRemove);
    toolBar->addSeparator();
    toolBar->addAction(actionMove);
    toolBar->addAction(actionZoom);
    toolBar->addWidget(spacer);
    toolBar->addAction(actionOpenSettings);

    addToolBar(Qt::LeftToolBarArea, toolBar);
    toolBar->setEnabled(false);
}

void MainWindow::createPageView()
{
    pageView = new PageView(this);
    pageView->setStyleSheet(
        "QGraphicsView"
        "{border: none; outline: none; background-color: white;}");

    connect(pageView,
            &PageView::canvasZoomChanged,
            this,
            &MainWindow::onCanvasZoomChanged);
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

    translationList->setEnabled(false);
    translationEditor->setEnabled(false);

    connect(translationList->translationList->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            translationEditor,
            &TranslationEditor::onSelectionChanged);
}

void MainWindow::createActions()
{
    actionOpen = new QAction("Open");
    actionOpen->setShortcut(QKeySequence::Open);
    actionOpen->setStatusTip("Open an existing file");
    connect(actionOpen, &QAction::triggered, this, &MainWindow::openFile);

    actionClose = new QAction("Close");
    actionClose->setShortcut(QKeySequence::Close);
    actionClose->setStatusTip("Close current working file");
    connect(actionClose, &QAction::triggered, this, &MainWindow::closeFile);

    actionQuit = new QAction("Quit");
    actionQuit->setShortcut(QKeySequence::Quit);
    actionQuit->setStatusTip("Quit application");
    connect(actionQuit, &QAction::triggered, this, &QCoreApplication::quit);

    actionFitInWindow = new QAction("Fit in Window");
    actionFitInWindow->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0));
    connect(actionFitInWindow,
            &QAction::triggered,
            pageView,
            &PageView::fitInWindow);

    actionActualSize = new QAction("Actual Size");
    actionActualSize->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_1));
    connect(actionActualSize,
            &QAction::triggered,
            pageView,
            &PageView::resetZoom);

    actionZoomIn = new QAction("Zoom In");
    actionZoomIn->setShortcut(QKeySequence::ZoomIn);
    connect(actionZoomIn, &QAction::triggered, pageView, &PageView::zoomIn);

    actionZoomOut = new QAction("Zoom Out");
    actionZoomOut->setShortcut(QKeySequence::ZoomOut);
    connect(actionZoomOut, &QAction::triggered, pageView, &PageView::zoomOut);

    actionSelect = new QAction(QIcon(":/resources/icons/select.svg"), "Select");
    actionDirectSelect = new QAction(QIcon(
                                         ":/resources/icons/select-direct.svg"),
                                     "Direct select");
    actionAdd = new QAction(QIcon(":/resources/icons/add.svg"), "Add");
    actionMerge = new QAction(QIcon(":/resources/icons/merge.svg"), "Merge");
    actionSplit = new QAction(QIcon(":/resources/icons/split.svg"), "Split");
    actionRemove = new QAction(QIcon(":/resources/icons/erase.svg"), "Remove");
    actionMove = new QAction(QIcon(":/resources/icons/hand.svg"), "Move");
    actionZoom = new QAction(QIcon(":/resources/icons/zoom.svg"), "Zoom");
    actionOpenSettings = new QAction(QIcon(":/resources/icons/settings.svg"),
                                     "Settings");

    canvasActions = new QActionGroup(this);
    canvasActions->addAction(actionSelect);
    canvasActions->addAction(actionDirectSelect);
    canvasActions->addAction(actionAdd);
    canvasActions->addAction(actionMerge);
    canvasActions->addAction(actionSplit);
    canvasActions->addAction(actionRemove);
    canvasActions->addAction(actionMove);
    canvasActions->addAction(actionZoom);

    for (auto action : canvasActions->actions()) {
        action->setCheckable(true);
        connect(action,
                &QAction::triggered,
                this,
                &MainWindow::onCanvasActionChanged);
    }
}

void MainWindow::openFile()
{
    QString filePath
        = QFileDialog::getOpenFileName(this,
                                       "Open Image",
                                       lastFileDialogDir.path(),
                                       "Image Files (*.png *.jpg *.bmp)");

    if (filePath.isNull() || filePath == currFilePath) return;

    if (isFileOpened) closeFile();

    currFilePath = filePath;
    lastFileDialogDir = QDir(filePath);
    isFileOpened = true;

    translationList->setEnabled(true);
    translationEditor->setEnabled(true);

    pageView->loadPage(filePath, translations);

    fileNameLabel->setText(QFileInfo(filePath).fileName());
    for (auto child : statusBar()->children())
        static_cast<QWidget *>(child)->show();

    toolBar->setEnabled(true);
    if (!currCanvasAction) currCanvasAction = canvasActions->actions().at(0);
    currCanvasAction->setChecked(true);
}

void MainWindow::closeFile()
{
    if (!isFileOpened) return;

    pageView->clearPage();
    translations->removeRows(0, translations->rowCount());

    for (auto child : statusBar()->children())
        static_cast<QWidget *>(child)->hide();

    currCanvasAction->setChecked(false);
    toolBar->setEnabled(false);

    translationList->setEnabled(false);
    translationEditor->setEnabled(false);

    currFilePath = "";
    isFileOpened = false;
}

void MainWindow::onCanvasZoomChanged(qreal scaleFactor)
{
    qreal percent = scaleFactor * 100;
    QString text = "Zoom: " + QString::number(percent, 'd', 0) + "%";
    zoomLabel->setText(text);
}

void MainWindow::onCanvasActionChanged()
{
    currCanvasAction = canvasActions->checkedAction();
}
