#ifndef TRANSLATIONSVIEW_H
#define TRANSLATIONSVIEW_H

#include <QDropEvent>
#include <QHeaderView>
#include <QObject>
#include <QTableView>

class TranslationsView : public QTableView
{
    Q_OBJECT

public:
    TranslationsView(QWidget *parent = nullptr);

protected:
    QModelIndexList selectedIndexes() const override;
    void dropEvent(QDropEvent *e) override;
    void startDrag(Qt::DropActions supportedActions) override;

private:
    QItemSelection droppedSelection;
    bool dropAccepted = false;
};

#endif // TRANSLATIONSVIEW_H
