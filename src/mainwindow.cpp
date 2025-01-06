#include "mainwindow.h"
#include "translationrect.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    translations(new TranslationsModel),
    proxy(new TranslationsProxy),
    reader(new Reader),
    ui(new Ui::MainWindow),
    lastFileDialogDir(QDir().home())
{
    ui->setupUi(this);

    setupActions();
    setupStatusBar();
    setupToolBar();
    setupTableView();

    connect(ui->pageView, &PageView::canvasZoomChanged, this,
            &MainWindow::onCanvasZoomChanged);
    connect(translations, &QAbstractItemModel::rowsMoved, this,
            &MainWindow::onRowsMoved);
    connect(ui->tableView->selectionModel(),
            &QItemSelectionModel::selectionChanged, this,
            &MainWindow::onSelectionChanged);
    connect(ui->sourceEdit, &QTextEdit::textChanged, this,
            &MainWindow::onTextChanged);
    connect(ui->targetEdit, &QTextEdit::textChanged, this,
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
    connect(ui->actionQuit, &QAction::triggered, this, &QCoreApplication::quit,
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

    tools = new QActionGroup(this);
    tools->addAction(actionSelect);
    tools->addAction(actionDirectSelect);
    tools->addAction(actionAdd);
    tools->addAction(actionMerge);
    tools->addAction(actionSplit);
    tools->addAction(actionRemove);
    tools->addAction(actionMove);
    tools->addAction(actionZoom);

    for (auto action : tools->actions()) {
        action->setCheckable(true);
        connect(action, &QAction::triggered, this,
                &MainWindow::onCanvasActionChanged);
    }
}

void MainWindow::setupTableView()
{
    proxy->setSourceModel(translations);
    ui->tableView->setModel(proxy);
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
    QString filePath =
        QFileDialog::getOpenFileName(this, "Open Image",
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
    if (translations->rowCount() != 0) {
        ui->tableView->setEnabled(true);
        ui->statusFilter->setEnabled(true);
        ui->toolBar->setEnabled(true);
    }
    if (!currTool) currTool = tools->actions().at(0);
    currTool->setChecked(true);
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
    clearAndDisableEditorPane();
    currTool->setChecked(false);
}

void MainWindow::onCanvasActionChanged() { currTool = tools->checkedAction(); }

void MainWindow::onCanvasZoomChanged(qreal scaleFactor)
{
    qreal percent = scaleFactor * 100;
    QString text = "Zoom: " + QString::number(percent, 'd', 0) + "%";
    zoomLabel->setText(text);
}

void MainWindow::updateProgress()
{
    int count = 0, rows = translations->rowCount();
    for (int row = 0; row < rows; ++row) {
        count += translations->data(translations->index(row, COMPLETED))
                     .toBool();
    }
    progressLabel->setText("Finished: "
                           + QString::number((qreal) count / rows * 100, 'd', 0)
                           + "%");
}

void MainWindow::onRowsMoved(const QModelIndex &parent, int start, int end,
                             const QModelIndex &destination, int row)
{
    qInfo() << parent << start << end << destination << row;
}

void MainWindow::onSelectionChanged(const QItemSelection &selected,
                                    const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);

    QModelIndexList selection = ui->tableView->selectionModel()
                                    ->selectedIndexes();
    if (selection.isEmpty()) {
        clearAndDisableEditorPane();
        return;
    }

    ui->sourceEdit->setPlainText(selection.at(TEXT).data().toString());
    ui->targetEdit->setPlainText(selection.at(TRANSLATION).data().toString());
    ui->completeButton->setChecked(selection.at(COMPLETED).data().toBool());
    updateRectInfo(selection.at(BOUNDS).data().toRect());
    setEnabledEditorPane(true, ui->completeButton->isChecked());
}

void MainWindow::onTextChanged()
{
    QModelIndexList selection = ui->tableView->selectionModel()
                                    ->selectedIndexes();
    if (selection.isEmpty()) return;

    QTextEdit *editor = dynamic_cast<QTextEdit *>(sender());
    int col = (editor == ui->sourceEdit) ? TEXT : TRANSLATION;
    ui->tableView->model()->setData(selection.at(col), editor->toPlainText());
}

void MainWindow::onCompleteButtonClicked(bool checked)
{
    QModelIndexList selection, newSelection;

    selection = ui->tableView->selectionModel()->selection().indexes();
    if (selection.isEmpty()) return;

    ui->tableView->model()->setData(selection.at(COMPLETED), checked);
    updateProgress();

    newSelection = ui->tableView->selectionModel()->selection().indexes();
    if (!newSelection.isEmpty())
        setEnabledEditorPane(true, newSelection.at(COMPLETED).data().toBool());
}

void MainWindow::onFilterChanged(QString filter)
{
    QModelIndexList selection, newSelection;
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();

    selection = selectionModel->selectedIndexes();
    proxy->filter = filter;
    proxy->invalidate();
    newSelection = selectionModel->selectedIndexes();

    if (!selection.isEmpty() && newSelection.isEmpty()) {
        selectionModel->clearSelection();
        clearAndDisableEditorPane();
    }
}

void MainWindow::setEnabledEditorPane(bool enabled, bool completed)
{
    bool isEditable = completed ? false : enabled;
    ui->translateButton->setEnabled(isEditable);
    ui->sourceEdit->setEnabled(isEditable);
    ui->targetEdit->setEnabled(isEditable);
    ui->completeButton->setEnabled(enabled);

    if (!isEditable) {
        for (auto editor : { ui->sourceEdit, ui->targetEdit }) {
            QTextCursor cursor = editor->textCursor();
            if (cursor.hasSelection()) {
                cursor.clearSelection();
                editor->setTextCursor(cursor);
            }
        }
    }
}

void MainWindow::clearEditorPane()
{
    ui->sourceEdit->clear();
    ui->targetEdit->clear();
    ui->completeButton->setChecked(false);
    updateRectInfo(QRect());
}

void MainWindow::clearAndDisableEditorPane()
{
    clearEditorPane();
    setEnabledEditorPane(false);
}

void MainWindow::updateRectInfo(QRect rect)
{
    ui->xValueLabel->setText(QString::number(rect.left()));
    ui->yValueLabel->setText(QString::number(rect.top()));
    ui->widthValueLabel->setText(QString::number(rect.width()));
    ui->heightValueLabel->setText(QString::number(rect.height()));
}
