#include "translationsview.h"

TranslationsView::TranslationsView(QWidget *parent) : QTableView(parent) {}

QModelIndexList TranslationsView::selectedIndexes() const
{
    // Override to include hidden items (columns) in the selection
    return selectionModel()->selectedIndexes();
}

void TranslationsView::startDrag(Qt::DropActions supportedActions)
{
    dropAccepted = false;

    QTableView::startDrag(supportedActions); // startDrag() calls dropEvent(),
                                             // thus dropAccepted gets updated
    if (dropAccepted)
        selectionModel()->select(droppedSelection,
                                 QItemSelectionModel::ClearAndSelect
                                     | QItemSelectionModel::Rows);
}

void TranslationsView::dropEvent(QDropEvent *e)
{
    droppedSelection.clear();

    const QMetaObject::Connection monitor =
        connect(model(), &QAbstractItemModel::rowsInserted, this,
                [this](const QModelIndex &, int first, int last) {
                    droppedSelection = QItemSelection(model()->index(first, 0),
                                                      model()->index(last, 0));
                });

    QTableView::dropEvent(e);

    if (e->isAccepted() && e->dropAction() == Qt::MoveAction
        && !droppedSelection.isEmpty())
        dropAccepted = true;

    disconnect(monitor);
}
