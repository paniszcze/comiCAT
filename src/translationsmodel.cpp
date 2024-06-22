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
    Q_UNUSED(column);

    if (row == -1) {
        row = rowCount();
    }

    bool ret = false;

    if (canDropMimeData(data, action, row, column, parent))
    {
        qInfo() << "canDropMimeData" << action;
        beginMoveRows(parent, row, row, {}, row - 1);
        ret = QStandardItemModel::dropMimeData(data, action, row, 0, parent);
        endMoveRows();
    }

    return ret;
}

Qt::DropActions TranslationsModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::ItemFlags TranslationsModel::flags(const QModelIndex &index) const
{
    return index.isValid()
               ? (QStandardItemModel::flags(index) & ~Qt::ItemIsDropEnabled)
               : (QStandardItemModel::flags(index) | Qt::ItemIsDropEnabled);
}
