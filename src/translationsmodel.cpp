#include "translationsmodel.h"

TranslationsModel::TranslationsModel(QObject *parent)
    : QStandardItemModel{parent}
{
    setColumnCount(HEADERS_COUNT);
    setHeaderData(TEXT, Qt::Horizontal, QObject::tr("Text"));
    setHeaderData(TRANSLATION, Qt::Horizontal, QObject::tr("Translation"));
    setHeaderData(BOUNDS, Qt::Horizontal, QObject::tr("Bounds"));
    setHeaderData(COMPLETED, Qt::Horizontal, QObject::tr(""));
}

void TranslationsModel::addTranslation(Translation translation)
{
    int row = rowCount();
    insertRow(row);
    setData(index(row, TEXT), translation.sourceText);
    setData(index(row, TRANSLATION), translation.targetText);
    setData(index(row, BOUNDS), translation.bounds);
    setData(index(row, COMPLETED), translation.isCompleted);
}

// TODO: drag'n'dropping of rows in the table view never worked properly,
//       needs to be fixed (with proxy model in mind!)
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

QVariant TranslationsModel::data(
    const QModelIndex &index, int role) const
{
    int col = index.column();
    if (col == COMPLETED && role == Qt::DecorationRole) {
        if (QStandardItemModel::data(index, Qt::DisplayRole).toBool())
            return QIcon(":/resources/icons/check.svg");
        else return "";
    }
    return QStandardItemModel::data(index, role);
}
