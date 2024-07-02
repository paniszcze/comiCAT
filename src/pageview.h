#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QDebug>
#include <QEvent>
#include <QGestureEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <QList>
#include <QObject>
#include <QPinchGesture>
#include <QPixmap>
#include <QRect>

#include "reader.h"

static const qreal MAX_SCALE = 20.0;
static const qreal MIN_SCALE = 0.03;

class PageView : public QGraphicsView
{
    Q_OBJECT

public:
    PageView(QWidget *parent = nullptr);
    ~PageView();

    void loadPage(QString filepath, QStandardItemModel *translations);
    bool event(QEvent *event) override;
    bool gestureEvent(QGestureEvent *event);
    void setScaleValue(qreal factor);

protected:
    void pinchTriggered(QPinchGesture *gesture);

signals:
    void canvasZoomChanged(qreal percent);

public:
    Reader *reader;

private:
    QString currentPath = "";
    QImage *currentImage = nullptr;
    QGraphicsPixmapItem *pixmapItem = nullptr;
    qreal scaleValue = 1.0;
};

#endif // PAGEVIEW_H
