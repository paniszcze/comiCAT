#include "translationsview.h"

TranslationsView::TranslationsView(QWidget *parent)
    : QTableView(parent)
{}

QModelIndexList TranslationsView::selectedIndexes() const
{
    // Override needed to include hidden items (columns)
    // in the selection, since we want to perform operations
    // on the entire rows
    return selectionModel()->selectedIndexes();
}
