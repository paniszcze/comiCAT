#include "translationsview.h"
#include "translationsviewstyle.h"

TranslationsView::TranslationsView(QWidget *parent)
    : QTableView(parent)
{
    setStyle(new TranslationsViewStyle(style()));
}

void TranslationsView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_S)
    {
        qInfo() << QString(10, '-');
        int rows = model()->rowCount();
        for (int r = 0; r < rows; ++r)
        {
            const QModelIndex i = model()->index(r, 0);
            qInfo() << model()->data(i);
        }
        qInfo() << QString(10, '-');
    }

    return QTableView::keyReleaseEvent(event);
}

void TranslationsView::onRowsMoved(const QModelIndex &sourceParent,
                                   int sourceStart,
                                   int sourceEnd,
                                   const QModelIndex &destinationParent,
                                   int destinationRow)
{
    qInfo() << "onRowsMoved";
    selectionModel()->select(sourceParent, QItemSelectionModel::Rows);
}

void TranslationsView::onRowsRemoved()
{
    qInfo() << "onRowsRemoved";
}

void TranslationsView::onRowsInserted(const QModelIndex &parent,
                                      int first,
                                      int last)
{
    Q_UNUSED(parent);
    qInfo() << "onRowsInserted" << first << last;
}
