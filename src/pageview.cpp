#include "pageview.h"
#include "translationrect.h"

PageView::PageView(QWidget *parent)
    : QGraphicsView(parent)
{
    reader = new Reader();

    setScene(new QGraphicsScene);
    grabGesture(Qt::PinchGesture);
    setAttribute(Qt::WA_AcceptTouchEvents);
}

PageView::~PageView()
{
    if (reader) delete reader;
}

void PageView::loadPage(QString filepath, QStandardItemModel *translations)
{
    if (filepath == currentPath || !scene()) return;

    if (currentImage) {
        delete currentImage;
        currentImage = nullptr;
        translations->removeRows(0, translations->rowCount());
    }

    currentImage = new QImage(filepath);
    if (currentImage) {
        scene()->clear();
        resetTransform();
        scaleValue = 1.0;

        currentPath = filepath;
        pixmapItem = scene()->addPixmap(QPixmap(currentPath));
        pixmapItem->setTransformationMode(Qt::SmoothTransformation);
        setSceneRect(pixmapItem->boundingRect());

        if (pixmapItem->boundingRect().width() > width()
            || pixmapItem->boundingRect().height() > height())
        {
            fitInView(pixmapItem, Qt::KeepAspectRatio);
            scaleValue = transform().m11();
            if (scaleValue > MAX_SCALE) {
                resetTransform();
                scale(MAX_SCALE, MAX_SCALE);
                scaleValue = MAX_SCALE;
            }
        }
        emit canvasZoomChanged(scaleValue * 100);

        QList<QRect> resultRecs = reader->readImg(filepath, translations);
        for (auto rect : resultRecs) {
            scene()->addItem(new TranslationRect{rect});
        }
    }
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
        setScaleValue(gesture->scaleFactor());
}

void PageView::setScaleValue(qreal factor)
{
    if (currentPath.isEmpty()) return;

    scaleValue *= factor;
    scale(factor, factor);

    if (factor < 1 && scaleValue < MIN_SCALE) {
        const qreal minv = MIN_SCALE / scaleValue;
        scale(minv, minv);
        scaleValue *= minv;
    } else if (factor > 1 && scaleValue > MAX_SCALE) {
        const qreal maxv = MAX_SCALE / scaleValue;
        scale(maxv, maxv);
        scaleValue *= maxv;
    }

    emit canvasZoomChanged(scaleValue * 100);
}
