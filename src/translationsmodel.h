#ifndef TRANSLATIONSMODEL_H
#define TRANSLATIONSMODEL_H

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
};

#endif // TRANSLATIONSMODEL_H
