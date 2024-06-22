#ifndef TRANSLATIONSVIEW_H
#define TRANSLATIONSVIEW_H

#include <QItemSelection>
#include <QKeyEvent>
#include <QModelIndex>
#include <QObject>
#include <QTableView>
#include <QWidget>

class TranslationsView : public QTableView
{
    Q_OBJECT

public:
    TranslationsView(QWidget *parent = nullptr);

    virtual void keyReleaseEvent(QKeyEvent *event) override;

public slots:
    void onRowsMoved(const QModelIndex &sourceParent,
                     int sourceStart,
                     int sourceEnd,
                     const QModelIndex &destinationParent,
                     int destinationRow);
    void onRowsRemoved();
    void onRowsInserted(const QModelIndex &parent, int first, int last);
};

#endif // TRANSLATIONSVIEW_H
