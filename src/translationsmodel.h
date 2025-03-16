#ifndef TRANSLATIONSMODEL_H
#define TRANSLATIONSMODEL_H

#include <QObject>
#include <QRect>
#include <QStandardItemModel>
#include <QString>

enum ModelHeaders { TEXT, TRANSLATION, BOUNDS, COMPLETED, HEADERS_COUNT };

struct Translation
{
    QString sourceText;
    QString targetText;
    QRect bounds;
    bool isCompleted;
};

class TranslationsModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit TranslationsModel(QObject *parent = nullptr);
    ~TranslationsModel();

    static TranslationsModel *instance();

    int addTranslation(Translation translation);

    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                              int row, int column,
                              const QModelIndex &parent) override;
    virtual Qt::DropActions supportedDropActions() const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
};

#endif // TRANSLATIONSMODEL_H
