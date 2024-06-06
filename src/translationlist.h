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

class TranslationList : public QWidget
{
    Q_OBJECT

public:
    TranslationList(QWidget *parent = nullptr,
                    QStandardItemModel *model = nullptr);

    QComboBox *statusCombo;
    QTableView *translationList;

private:
    void paintEvent(QPaintEvent *) override;
};

#endif // TRANSLATIONLIST_H
