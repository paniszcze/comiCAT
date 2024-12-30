#ifndef TRANSLATIONSPROXY_H
#define TRANSLATIONSPROXY_H

#include <QDebug>
#include <QObject>
#include <QSortFilterProxyModel>

class TranslationsProxy : public QSortFilterProxyModel
{
public:
    TranslationsProxy();

    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent) const override;

    QString filter;
};

#endif // TRANSLATIONSPROXY_H
