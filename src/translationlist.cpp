#include "translationlist.h"

TranslationList::TranslationList(QWidget *parent,
                                 QStandardItemModel *translations)
    : QWidget{parent}
{
    setObjectName("TranslationList");
    setStyleSheet(
        "QWidget#TranslationList {border-radius: 4; border: 1px solid #e0e0e0;}"
        "QHeaderView {background-color: white; color:#5c5c5c;}"
        "QHeaderView::section {background-color: white;"
        "border: 0; border-bottom: 1px solid #e0e0e0; padding-bottom: 4px;}"
        "QComboBox {font-size: 12px; color: #3c3c3c;}"
        "QScrollBar::handle {background-color: white;}"
        "QLabel#Translations {padding-left: 1px;}"
        "QLabel#StatusLabel {color: #5c5c5c;font-size: 11px;}"
        "QTableView {border: 0; background-color: white; font-size: 11px;}"
        "QTableView::item {border-bottom: 1px solid #e0e0e0; padding: 4px 0;}");

    QLabel *title = new QLabel("Translations");
    title->setObjectName("Translations");
    statusCombo = new QComboBox();
    statusCombo->addItems({"All", "Working", "Finished"});
    QLabel *statusLabel = new QLabel("Show:");
    statusLabel->setObjectName("StatusLabel");
    statusLabel->setBuddy(statusCombo);

    translationList = new QTableView;

    // selection behaviour
    translationList->setSelectionMode(QAbstractItemView::SingleSelection);
    translationList->setSelectionBehavior(QAbstractItemView::SelectRows);
    translationList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // drag'n'drop behaviour
    translationList->setDragEnabled(true);
    translationList->setDropIndicatorShown(true);
    translationList->setDefaultDropAction(Qt::MoveAction);
    translationList->setDragDropMode(QAbstractItemView::InternalMove);
    translationList->setDragDropOverwriteMode(false);

    translationList->setModel(translations);
    translationList->hideColumn(2);
    translationList->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    translationList->horizontalHeader()->setHighlightSections(false);
    translationList->verticalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    translationList->verticalHeader()->setVisible(false);
    translationList->setShowGrid(false);

    QHBoxLayout *header = new QHBoxLayout();
    header->addWidget(title, 1, Qt::AlignLeft);
    header->addWidget(statusLabel);
    header->addWidget(statusCombo);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(header);
    layout->addWidget(translationList);

    setLayout(layout);
}

void TranslationList::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
