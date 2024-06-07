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

#include "page.h"
#include "reader.h"
#include "translationrect.h"

class PageView : public QGraphicsView
{
    Q_OBJECT

public:
    PageView(QWidget *parent = nullptr);
    ~PageView();

    Page *mPage;
    Reader *mReader;

    void loadImg(QString filename, QStandardItemModel *translations);
};

#endif // PAGEVIEW_H
