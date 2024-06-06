#include "translationsmodel.h"

TranslationsModel::TranslationsModel(QObject *parent)
    : QStandardItemModel{parent}
{
    setColumnCount(3);
    setHeaderData(0, Qt::Horizontal, QObject::tr("Text"));
    setHeaderData(1, Qt::Horizontal, QObject::tr("Translation"));
    setHeaderData(2, Qt::Horizontal, QObject::tr("Bounds"));
}

bool TranslationsModel::dropMimeData(const QMimeData *data,
                                     Qt::DropAction action,
                                     int row,
                                     int column,
                                     const QModelIndex &parent)
{
    Q_UNUSED(parent);
    Q_UNUSED(column);

    if (row == -1) {
        row = rowCount();
    }

    return QAbstractItemModel::dropMimeData(data, action, row, 0, parent);
}

Qt::DropActions TranslationsModel::supportedDropActions() const
{
    return Qt::MoveAction;
}
