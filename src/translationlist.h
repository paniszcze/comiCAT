#ifndef TRANSLATIONLIST_H
#define TRANSLATIONLIST_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPainter>
#include <QStandardItemModel>
#include <QStyleOption>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>

#include "translationsmodel.h"
#include "translationsview.h"

class TranslationList : public QWidget
{
    Q_OBJECT

public:
    TranslationList(QWidget *parent = nullptr,
                    TranslationsModel *model = nullptr);

public slots:
    void onItemNeedsUpdate(QModelIndex itemIndex, QString updatedText);

private:
    void paintEvent(QPaintEvent *) override;

public:
    TranslationsModel *translations;
    TranslationsView *translationsView;
    QComboBox *statusCombo;
};

#endif // TRANSLATIONLIST_H
