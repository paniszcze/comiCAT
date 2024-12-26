#include "mainwindow.h"
#include "translationrect.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(
    QWidget *parent) :
    QMainWindow(parent),
    translations(new TranslationsModel),
    reader(new Reader),
    ui(new Ui::MainWindow)
// isFileOpened(false),
// currFilePath(""),
// lastFileDialogDir(QDir().home()),
// currCanvasAction(nullptr)
{
    ui->setupUi(this);

    ui->tableView->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    ui->tableView->setModel(translations);
    ui->tableView->hideColumn(2);

    ui->pageView->loadPage(
        "/Users/danny/Downloads/comics/killing-my-childhood.jpg");

    // connect(ui->pageView,
    //         &PageView::canvasZoomChanged,
    //         this,
    //         &MainWindow::onCanvasZoomChanged);
    connect(ui->tableView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            translationEditor,
            &TranslationEditor::onSelectionChanged);
    connect(translationEditor,
            &TranslationEditor::itemNeedsUpdate,
            this,
            &MainWindow::onItemNeedsUpdate);

    // createActions();
    // createMenuBar();
    // createStatusBar();
    // createToolBar();
}

MainWindow::~MainWindow()
{
    delete translations;
    delete reader;
    delete ui;
}

void MainWindow::createMenuBar()
{
    menuBar = new QMenuBar();
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
    projectInfo->setObjectName("info");
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

    updateStatusBarInfo();
}

void MainWindow::updateStatusBarInfo(QString fileName)
{
    fileNameLabel->setText(fileName);
    QWidget *projectInfo = statusBar()->findChild<QWidget *>("info");

    if (!isFileOpened) {
        fileNameLabel->hide();
        projectInfo->hide();
    } else {
        fileNameLabel->show();
        projectInfo->show();
    }
}

void MainWindow::createToolBar()
{
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
    // connect(actionFitInWindow,
    //         &QAction::triggered,
    //         ui->pageView,
    //         &PageView::fitInWindow);

    actionActualSize = new QAction("Actual Size");
    actionActualSize->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_1));
    // connect(actionActualSize,
    //         &QAction::triggered,
    //         ui->pageView,
    //         &PageView::resetZoom);

    actionZoomIn = new QAction("Zoom In");
    actionZoomIn->setShortcut(QKeySequence::ZoomIn);
    // connect(actionZoomIn, &QAction::triggered, ui->pageView, &PageView::zoomIn);

    actionZoomOut = new QAction("Zoom Out");
    actionZoomOut->setShortcut(QKeySequence::ZoomOut);
    // connect(actionZoomOut,
    //         &QAction::triggered,
    //         ui->pageView,
    //         &PageView::zoomOut);

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
    isFileOpened = true;

    currFilePath = filePath;
    lastFileDialogDir = QDir(filePath);

    toolBar->setEnabled(true);
    if (!currCanvasAction) currCanvasAction = canvasActions->actions().at(0);
    currCanvasAction->setChecked(true);

    updateStatusBarInfo(QFileInfo(filePath).fileName());

    ui->pageView->loadPage(filePath);
    QList<Translation> ocrResults = reader->readImg(filePath);
    if (!ocrResults.empty()) {
        for (auto &item : ocrResults) {
            ui->pageView->scene()->addItem(new TranslationRect{item.bounds});
            translations->addTranslation(item);
        }
    }
    ui->tableView->setEnabled(true);
    ui->statusFilter->setEnabled(true);
}

void MainWindow::closeFile()
{
    if (!isFileOpened) return;

    ui->pageView->clearPage();
    translations->removeRows(0, translations->rowCount());

    for (auto child : statusBar()->children())
        static_cast<QWidget *>(child)->hide();

    currCanvasAction->setChecked(false);
    toolBar->setEnabled(false);

    ui->tableView->setEnabled(false);
    ui->statusFilter->setEnabled(false);

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

// <--- translationList
void MainWindow::onItemNeedsUpdate(
    QModelIndex itemIndex, QString updatedText)
{
    if (ui->tableView->selectionModel()->isRowSelected(itemIndex.row(),
                                                       QModelIndex()))
        translations->setData(itemIndex, updatedText);
}
// translationList --->
