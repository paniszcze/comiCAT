#include "translationsmodel.h"

TranslationsModel::TranslationsModel(QObject *parent)
    : QStandardItemModel{parent}
{
    setColumnCount(3);
    setHeaderData(0, Qt::Horizontal, QObject::tr("Text"));
    setHeaderData(1, Qt::Horizontal, QObject::tr("Translation"));
    setHeaderData(2, Qt::Horizontal, QObject::tr("Bounds"));
}

void TranslationsModel::addTranslation(Translation translation)
{
    int row = rowCount();
    insertRow(row);
    setData(index(row, 0), translation.sourceText);
    setData(index(row, 1), translation.targetText);
    setData(index(row, 2), translation.bounds);
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

    return QStandardItemModel::dropMimeData(data, action, row, 0, parent);
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
