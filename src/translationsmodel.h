#ifndef TRANSLATIONSMODEL_H
#define TRANSLATIONSMODEL_H

#include <QDebug>
#include <QModelIndex>
#include <QStandardItemModel>

class TranslationsModel : public QStandardItemModel
{
public:
    explicit TranslationsModel(QObject *parent = nullptr);

    virtual bool dropMimeData(const QMimeData *data,
                              Qt::DropAction action,
                              int row,
                              int column,
                              const QModelIndex &parent) override;
    virtual Qt::DropActions supportedDropActions() const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
};

#endif // TRANSLATIONSMODEL_H
