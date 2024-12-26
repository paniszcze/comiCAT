#include "translationsmodel.h"

TranslationsModel::TranslationsModel(QObject *parent)
    : QStandardItemModel{parent}
{
    setColumnCount(HEADERS_COUNT);
    setHeaderData(TEXT, Qt::Horizontal, QObject::tr("Text"));
    setHeaderData(TRANSLATION, Qt::Horizontal, QObject::tr("Translation"));
    setHeaderData(BOUNDS, Qt::Horizontal, QObject::tr("Bounds"));
}

void TranslationsModel::addTranslation(Translation translation)
{
    int row = rowCount();
    insertRow(row);
    setData(index(row, TEXT), translation.sourceText);
    setData(index(row, TRANSLATION), translation.targetText);
    setData(index(row, BOUNDS), translation.bounds);
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
