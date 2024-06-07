#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QDebug>
#include <QEvent>
#include <QGestureEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QPen>
#include <QPixmap>

#include "page.h"
#include "reader.h"
#include "translationrect.h"

const qreal MAX_SCALE = 20.0;
const qreal MIN_SCALE = 0.03;

class PageView : public QGraphicsView
{
    Q_OBJECT

public:
    PageView(QWidget *parent = nullptr);
    ~PageView();

    void loadImg(QString filepath, QStandardItemModel *translations);
    bool event(QEvent *event) override;
    bool gestureEvent(QGestureEvent *event);
    void setScaleValue(qreal factor);

public slots:
    void resizeEvent(QResizeEvent *event) override;

protected:
    void handleGestureEvent(QGestureEvent *gesture);
    void pinchTriggered(QPinchGesture *gesture);

signals:
    void scaled(qreal percent);

public:
    Page *page;
    Reader *reader;

private:
    QString currentPath;
    QImage *currentImage;
    QGraphicsPixmapItem *pixmapItem{nullptr};
    qreal scaleValue = 1.0;
};

#endif // PAGEVIEW_H
