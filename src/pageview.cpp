#include "pageview.h"

PageView::PageView(QWidget *parent)
    : QGraphicsView(parent)
{
    setScene(new QGraphicsScene);
    grabGesture(Qt::PinchGesture);
    setAttribute(Qt::WA_AcceptTouchEvents);
}

PageView::~PageView() {}

void PageView::loadPage(QString filePath)
{
    if (filePath == currentPath || !scene()) return;

    currentImage = new QImage(filePath);
    if (!currentImage) return;

    currentPath = filePath;
    pixmapItem = scene()->addPixmap(QPixmap(currentPath));
    pixmapItem->setTransformationMode(Qt::SmoothTransformation);
    setSceneRect(pixmapItem->boundingRect());
    if (!QRectF(rect()).contains(sceneRect())) fitInWindow();
}

void PageView::clearPage()
{
    if (!scene()) return;

    resetZoom();
    setSceneRect(rect());
    scene()->clear();

    if (currentImage) {
        delete currentImage;
        currentImage = nullptr;
    }
    currentPath = "";
}

bool PageView::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent *>(event));

    return QGraphicsView::event(event);
}

bool PageView::gestureEvent(QGestureEvent *event)
{
    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));

    return true;
}

void PageView::pinchTriggered(QPinchGesture *gesture)
{
    if (gesture->changeFlags() & QPinchGesture::ScaleFactorChanged)
        setScaleFactor(gesture->scaleFactor());
}

void PageView::fitInWindow()
{
    resetZoom();

    QRectF vRect = rect();
    QRectF sRect = sceneRect();
    if (vRect.isEmpty() || sRect.isEmpty()) return;

    qreal xratio = vRect.width() / sRect.width();
    qreal yratio = vRect.height() / sRect.height();
    qreal ratio = qMin(xratio, yratio);

    setScaleFactor(ratio);
    centerOn(sceneRect().center());
}

void PageView::resetZoom() { setScaleFactor(1.0 / scaleFactor); }

void PageView::zoomIn() { setScaleFactor(ZOOM_STEP); }

void PageView::zoomOut() { setScaleFactor(1.0 / ZOOM_STEP); }

void PageView::setScaleFactor(qreal factor)
{
    if (currentPath.isEmpty()) return;

    scaleFactor *= factor;
    scale(factor, factor);

    if (factor < 1.0 && scaleFactor < MIN_SCALE) {
        const qreal minv = MIN_SCALE / scaleFactor;
        scale(minv, minv);
        scaleFactor *= minv;
    } else if (factor > 1.0 && scaleFactor > MAX_SCALE) {
        const qreal maxv = MAX_SCALE / scaleFactor;
        scale(maxv, maxv);
        scaleFactor *= maxv;
    }

    emit canvasZoomChanged(scaleFactor);
}
