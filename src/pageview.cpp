#include "pageview.h"

PageView::PageView(QWidget *parent)
    : QGraphicsView(parent)
{
    // create OCR instance
    reader = new Reader();

    // create and set graphics scene
    page = new Page();
    setScene(page);

    // enable pinch gesture
    grabGesture(Qt::PinchGesture);
    setAttribute(Qt::WA_AcceptTouchEvents);
}

PageView::~PageView()
{
    delete reader;
}

void PageView::loadImg(QString filename, QStandardItemModel *translations)
{
    QPixmap img = QPixmap(filename);
    QGraphicsPixmapItem *imgItem = page->addPixmap(img);
    fitInView(imgItem, Qt::KeepAspectRatio);

    QList<QRect> resultRecs = reader->readImg(filename, translations);

    for (auto rect : resultRecs) {
        page->addItem(new TranslationRect{rect});
    }
}

bool PageView::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture) {
        return gestureEvent(static_cast<QGestureEvent *>(event));
    }
    return QGraphicsView::event(event);
}

bool PageView::gestureEvent(QGestureEvent *event)
{
    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    return true;
}

void PageView::resizeEvent(QResizeEvent *event)
{
    return QGraphicsView::resizeEvent(event);
}

void PageView::handleGestureEvent(QGestureEvent *gesture)
{
    if (QGesture *pinch = gesture->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
}

void PageView::pinchTriggered(QPinchGesture *gesture)
{
    if (gesture->changeFlags() & QPinchGesture::ScaleFactorChanged) {
        setScaleValue(gesture->scaleFactor());
    }
}

void PageView::setScaleValue(qreal factor)
{
    scaleValue *= factor;
    scale(factor, factor);

    // keep the zoom inbetween MIN and MAX
    if (factor < 1 && scaleValue < MIN_SCALE) {
        const qreal minv = MIN_SCALE / scaleValue;
        scale(minv, minv);
        scaleValue *= minv;
    } else if (factor > 1 && scaleValue > MAX_SCALE) {
        const qreal maxv = MAX_SCALE / scaleValue;
        scale(maxv, maxv);
        scaleValue *= maxv;
    }

    emit scaled(scaleValue * 100);
}
