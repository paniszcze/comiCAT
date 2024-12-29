#include "mainwindow.h"
#include "translationrect.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(
    QWidget *parent) :
    QMainWindow(parent),
    translations(new TranslationsModel),
    reader(new Reader),
    x(0),
    y(0),
    width(0),
    height(0),
    ui(new Ui::MainWindow),
    isFileOpened(false),
    currFilePath(""),
    lastFileDialogDir(QDir().home()),
    currCanvasAction(nullptr),
    currSource(QModelIndex()),
    currTarget(QModelIndex())
{
    ui->setupUi(this);

    setupActions();
    setupStatusBar();
    setupToolBar();
    setupTableView();

    connect(ui->pageView,
            &PageView::canvasZoomChanged,
            this,
            &MainWindow::onCanvasZoomChanged);
    connect(ui->tableView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &MainWindow::onSelectionChanged);
    connect(ui->sourceEdit,
            &QTextEdit::textChanged,
            this,
            &MainWindow::onTextChanged);
    connect(ui->targetEdit,
            &QTextEdit::textChanged,
            this,
            &MainWindow::onTextChanged);
}

MainWindow::~MainWindow()
{
    delete translations;
    delete reader;
    delete ui;
}

void MainWindow::setupStatusBar()
{
    fileNameLabel = new QLabel("");
    zoomLabel = new QLabel("Zoom: 100%");
    pageLabel = new QLabel("Page 1 of 1");
    progressLabel = new QLabel("Finished: 0%");

    auto separator = []() { return new QLabel("|"); };

    QWidget *projectInfo = new QWidget();
    projectInfo->setObjectName("projectInfo");
    QHBoxLayout *infoLayout = new QHBoxLayout(projectInfo);
    infoLayout->setContentsMargins(10, 0, 0, 0);
    infoLayout->addWidget(zoomLabel);
    infoLayout->addWidget(separator());
    infoLayout->addWidget(pageLabel);
    infoLayout->addWidget(separator());
    infoLayout->addWidget(progressLabel);

    statusBar()->addWidget(fileNameLabel);
    statusBar()->addPermanentWidget(projectInfo);
    statusBar()->setContentsMargins(6, 0, 6, 0);
    statusBar()->setStyleSheet(
        "QStatusBar { min-height: 28px; background-color: white;"
        "border-top: 1px solid #e0e0e0; }"
        "QStatusBar::item { border: none; outline: none; }"
        "QLabel { border: none; outline: none;"
        "color: #828282; font-size: 11px; font-weight: 300; }"
    );

    updateStatusBarVisibility();
}

void MainWindow::updateStatusBarVisibility()
{
    QWidget *projectInfo = statusBar()->findChild<QWidget *>("projectInfo");
    if (!isFileOpened) {
        fileNameLabel->hide();
        projectInfo->hide();
    } else {
        fileNameLabel->show();
        projectInfo->show();
    }
}

void MainWindow::setupToolBar()
{
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    ui->toolBar->addAction(actionSelect);
    ui->toolBar->addAction(actionDirectSelect);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionAdd);
    ui->toolBar->addAction(actionMerge);
    ui->toolBar->addAction(actionSplit);
    ui->toolBar->addAction(actionRemove);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionMove);
    ui->toolBar->addAction(actionZoom);
    ui->toolBar->addWidget(spacer);
    ui->toolBar->addAction(actionSettings);
}

void MainWindow::setupActions()
{
    ui->actionOpen->setShortcut(QKeySequence::Open);
    ui->actionClose->setShortcut(QKeySequence::Close);
    ui->actionZoomIn->setShortcut(QKeySequence::ZoomIn);
    ui->actionZoomOut->setShortcut(QKeySequence::ZoomOut);
    ui->actionQuit->setShortcut(QKeySequence::Quit);
    connect(ui->actionQuit,
            &QAction::triggered,
            this,
            &QCoreApplication::quit,
            Qt::QueuedConnection);

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
    actionSettings = new QAction(QIcon(":/resources/icons/settings.svg"),
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

void MainWindow::setupTableView()
{
    ui->tableView->setModel(translations);
    ui->tableView->verticalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(COMPLETED,
                                                            QHeaderView::Fixed);
    ui->tableView->setColumnWidth(COMPLETED, 24);
    ui->tableView->hideColumn(BOUNDS);
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
    isFileOpened = true;
    lastFileDialogDir = QDir(filePath);

    ui->pageView->loadPage(filePath);
    QList<Translation> ocrResults = reader->readImg(filePath);
    if (!ocrResults.empty()) {
        for (auto &item : ocrResults) {
            ui->pageView->scene()->addItem(new TranslationRect{item.bounds});
            translations->addTranslation(item);
        }
    }

    fileNameLabel->setText(QFileInfo(filePath).fileName());
    updateStatusBarVisibility();
    ui->tableView->setEnabled(true);
    ui->statusFilter->setEnabled(true);
    ui->sourceEdit->setEnabled(true);
    ui->targetEdit->setEnabled(true);
    ui->translateButton->setEnabled(true);
    ui->completeButton->setEnabled(true);
    ui->toolBar->setEnabled(true);
    if (!currCanvasAction) currCanvasAction = canvasActions->actions().at(0);
    currCanvasAction->setChecked(true);
}

void MainWindow::closeFile()
{
    if (!isFileOpened) return;

    ui->pageView->clearPage();
    translations->removeRows(0, translations->rowCount());
    currFilePath = "";
    progressLabel->setText("Finished: 0%");
    isFileOpened = false;

    updateStatusBarVisibility();
    ui->toolBar->setEnabled(false);
    ui->tableView->setEnabled(false);
    ui->statusFilter->setEnabled(false);
    ui->sourceEdit->setEnabled(false);
    ui->targetEdit->setEnabled(false);
    ui->translateButton->setEnabled(false);
    ui->completeButton->setEnabled(false);
    currCanvasAction->setChecked(false);
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

void MainWindow::updateInfoBox()
{
    ui->xValueLabel->setText(QString::number(x));
    ui->yValueLabel->setText(QString::number(y));
    ui->widthValueLabel->setText(QString::number(width));
    ui->heightValueLabel->setText(QString::number(height));
}

void MainWindow::setInfoDetails(
    QRect rect)
{
    x = rect.left();
    y = rect.top();
    width = rect.width();
    height = rect.height();
}

void MainWindow::onSelectionChanged(
    const QItemSelection &selected, const QItemSelection &deselected)
{
    if (selected.isEmpty()) {
        if (deselected.isEmpty()) return;

        currSource = QModelIndex();
        currTarget = QModelIndex();
        ui->sourceEdit->clear();
        ui->targetEdit->clear();
        ui->completeButton->setChecked(false);
        setInfoDetails(QRect());
    } else {
        currSource = selected.indexes()[TEXT];
        currTarget = selected.indexes()[TRANSLATION];
        ui->sourceEdit->setPlainText(currSource.data().toString());
        ui->targetEdit->setPlainText(currTarget.data().toString());
        ui->completeButton->setChecked(
            selected.indexes()[COMPLETED].data().toBool());
        setInfoDetails(selected.indexes()[BOUNDS].data().toRect());
    }

    updateInfoBox();
}

void MainWindow::onTextChanged()
{
    if (sender() == ui->sourceEdit) {
        if (ui->tableView->selectionModel()->isRowSelected(currSource.row(),
                                                           QModelIndex()))
            translations->setData(currSource, ui->sourceEdit->toPlainText());
    } else {
        if (ui->tableView->selectionModel()->isRowSelected(currTarget.row(),
                                                           QModelIndex()))
            translations->setData(currTarget, ui->targetEdit->toPlainText());
    }
}

void MainWindow::onCompleteButtonClicked(
    bool checked)
{
    QItemSelection selection = ui->tableView->selectionModel()->selection();
    if (!selection.isEmpty()) {
        // update the model
        translations->setData(selection.indexes().at(COMPLETED), checked);
        // recalculate progress and update statusbar label
        int completeCount = 0;
        for (int row = 0; row < translations->rowCount(); ++row) {
            completeCount += translations
                                 ->data(translations->index(row, COMPLETED))
                                 .toBool();
        }
        qreal percent = (qreal) completeCount / translations->rowCount() * 100;
        progressLabel->setText("Finished: " + QString::number(percent, 'd', 0)
                               + "%");
    }
}
