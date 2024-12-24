#include "translationsview.h"
#include "translationsviewstyle.h"

TranslationsView::TranslationsView(QWidget *parent)
    : QTableView(parent)
{
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    horizontalHeader()->setHighlightSections(false);
    verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    verticalHeader()->setVisible(false);
    setShowGrid(false);
    setStyle(new TranslationsViewStyle(style()));

    // selection behaviour
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    // drag'n'drop behaviour
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setDefaultDropAction(Qt::MoveAction);
    setDragDropMode(QAbstractItemView::InternalMove);
    setDragDropOverwriteMode(false);
}

QModelIndexList TranslationsView::selectedIndexes() const
{
    return selectionModel()->selectedIndexes();
}
