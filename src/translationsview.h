#ifndef TRANSLATIONSVIEW_H
#define TRANSLATIONSVIEW_H

#include <QObject>
#include <QTableView>
#include <QWidget>

class TranslationsView : public QTableView
{
    Q_OBJECT

public:
    TranslationsView(QWidget *parent = nullptr);

protected:
    QModelIndexList selectedIndexes() const;
};

#endif // TRANSLATIONSVIEW_H
