#include "translationlist.h"

TranslationList::TranslationList(QWidget *parent, TranslationsModel *model) :
    QWidget(parent), translations(model)
{
    setObjectName("TranslationList");
    setStyleSheet(
        "QWidget#TranslationList {border-radius: 4; border: 1px solid #e0e0e0;}"
        "QLabel#Translations"
        "{padding-left: 1px; color: black; font-size: 13px;}"
        "QLabel#StatusLabel {color: #5c5c5c; font-size: 11px;}"
        "QComboBox {color: #3c3c3c; font-size: 12px;}"
        "QTableView {border: 0; background-color: white;"
        "font-weight: 300; font-size: 11px;}"
        "QTableView::item {border-bottom: 1px solid #e0e0e0;"
        "padding: 6px 0; color: black;}"
        "QTableView::item:selected {background-color: #f2f2f2;}"
        "QHeaderView {background-color: white; color:#5c5c5c;"
        "font-weight: 300;}"
        "QHeaderView::section {height: 20px; background-color: white;"
        "border: 0; border-bottom: 1px solid #e0e0e0; padding-bottom: 4px;}"
        "QScrollBar:vertical {margin: 24px 0px 0px 4px;}");

    QLabel *title = new QLabel("Translations");
    title->setObjectName("Translations");
    statusCombo = new QComboBox();
    statusCombo->addItems({"All", "Working", "Finished"});
    QLabel *statusLabel = new QLabel("Show:");
    statusLabel->setObjectName("StatusLabel");
    statusLabel->setBuddy(statusCombo);

    translationsView = new TranslationsView();
    translationsView->setModel(translations);
    translationsView->hideColumn(2);

    QHBoxLayout *header = new QHBoxLayout();
    header->addWidget(title, 1, Qt::AlignLeft);
    header->addWidget(statusLabel);
    header->addWidget(statusCombo);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(header);
    layout->addWidget(translationsView);

    setLayout(layout);
    setEnabled(false);
}

void TranslationList::onItemNeedsUpdate(QModelIndex itemIndex,
                                        QString updatedText)
{
    if (translationsView->selectionModel()->isRowSelected(itemIndex.row(),
                                                          QModelIndex()))
        translations->setData(itemIndex, updatedText);
}

void TranslationList::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
