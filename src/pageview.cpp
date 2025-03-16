#include "pageview.h"

PageView::PageView(QWidget *parent) :
    QGraphicsView(parent), page(new QGraphicsScene(this)), reader(new Reader)
{
    setScene(page);
    grabGesture(Qt::PinchGesture);
    setAttribute(Qt::WA_AcceptTouchEvents);
    setMouseTracking(true);

    model = TranslationsModel::instance();

    connect(model, &TranslationsModel::rowsInserted, this,
            [=](const QModelIndex &, int first, int last) {
                for (int row = first; row <= last; row++) {
                    QPersistentModelIndex index = model->index(row, BOUNDS);
                    TranslationRect *rect = new TranslationRect{
                        { 0, 0, 0, 0 },
                        index
                    };
                    rectDict.insert(index, rect);
                    scene()->addItem(rect);
                }
            });

    connect(model, &TranslationsModel::dataChanged, this,
            [=](const QModelIndex &topLeft, const QModelIndex &bottomRight) {
                if (topLeft.column() == BOUNDS) {
                    for (int row = topLeft.row(); row <= bottomRight.row();
                         row++) {
                        QPersistentModelIndex index = model->index(row, BOUNDS);
                        TranslationRect *rect = rectDict.value(index);
                        rect->setRect(
                            model->data(model->index(row, BOUNDS)).toRect());
                    }
                }
            });

    connect(model, &TranslationsModel::rowsAboutToBeRemoved, this,
            [=](const QModelIndex &, int first, int last) {
                for (int row = first; row <= last; row++) {
                    QPersistentModelIndex index = model->index(row, BOUNDS);
                    TranslationRect *rect = rectDict.value(index);
                    scene()->removeItem(rect);
                    rectDict.remove(index);
                    delete rect;
                }
            });

    // UNUSED SIGNALS
    // connect(model, &TranslationsModel::rowsRemoved, this,
    //         [=](const QModelIndex &, int first, int last) {
    //             qInfo() << "rowsRemoved" << first << last;
    //         });
    // connect(model, &TranslationsModel::rowsMoved, this,
    //         [=](const QModelIndex &, int first, int last) {
    //             qInfo() << "rowsMoved" << first << last;
    //         });
    // connect(model, &TranslationsModel::rowsAboutToBeInserted, this,
    //         [=](const QModelIndex &, int first, int last) {
    //             qInfo() << "rowsAboutToBeInserted" << first << last;
    //         });
    // connect(model, &TranslationsModel::rowsAboutToBeMoved, this,
    //         [=](const QModelIndex &, int first, int last) {
    //             qInfo() << "rowsAboutToBeMoved" << first << last;
    //         });
}

PageView::~PageView() { delete reader; }

void PageView::loadPage(QString filePath)
{
    currFilePath = filePath;
    currImage = new QImage(currFilePath);

    pixmapItem = scene()->addPixmap(QPixmap(currFilePath));
    pixmapItem->setTransformationMode(Qt::SmoothTransformation);
    setSceneRect(pixmapItem->boundingRect());
    if (!QRectF(rect()).contains(sceneRect())) fitInWindow();
}

void PageView::clearPage()
{
    resetZoom();
    setSceneRect(rect());
    scene()->clear();

    if (currImage) {
        delete currImage;
        currImage = nullptr;
    }
    currFilePath = "";
}

void PageView::readPage()
{
    if (currFilePath.isEmpty()) return;

    QList<Translation> ocrResults = reader->readImg(currFilePath);
    if (!ocrResults.empty())
        for (auto &result : ocrResults) model->addTranslation(result);
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
    if (currFilePath.isEmpty()) return;

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
