#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QObject>
#include <QPen>
#include <QPixmap>
#include <QWidget>

#include "reader.h"
#include "translationrect.h"

class PageView : public QWidget
{
    Q_OBJECT

public:
    PageView(QWidget *parent = nullptr);
    ~PageView();

    QGraphicsView *mPageView;
    QGraphicsScene *mPage;
    Reader *mReader;

    void loadImg(QString filename, QStandardItemModel *translations);
};

#endif // PAGEVIEW_H
