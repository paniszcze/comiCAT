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
#include <QRectF>

static const qreal MAX_SCALE = 20.0;
static const qreal MIN_SCALE = 0.03;
static const qreal ZOOM_STEP = 1.09;

class PageView : public QGraphicsView
{
    Q_OBJECT

public:
    PageView(QWidget *parent = nullptr);
    ~PageView();

    void loadPage(QString filepath);
    void clearPage();
    bool event(QEvent *event) override;
    bool gestureEvent(QGestureEvent *event);

public slots:
    void fitInWindow();
    void resetZoom();
    void zoomIn();
    void zoomOut();

signals:
    void canvasZoomChanged(qreal scaleFactor);

protected:
    void pinchTriggered(QPinchGesture *gesture);

private:
    void setScaleFactor(qreal factor);

private:
    QString currFilePath = "";
    QImage *currImage = nullptr;
    QGraphicsPixmapItem *pixmapItem = nullptr;
    qreal scaleFactor = 1.0;
};

#endif // PAGEVIEW_H
