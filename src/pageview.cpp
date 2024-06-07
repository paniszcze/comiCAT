#include "pageview.h"

PageView::PageView(QWidget *parent)
    : QGraphicsView(parent)
{
    mReader = new Reader();
    mPage = new Page();
    setScene(mPage);

    grabGesture(Qt::PinchGesture);
    setAttribute(Qt::WA_AcceptTouchEvents);
}

PageView::~PageView()
{
    delete mReader;
}

void PageView::loadImg(QString filename, QStandardItemModel *translations)
{
    QPixmap img = QPixmap(filename);
    QGraphicsPixmapItem *imgItem = mPage->addPixmap(img);
    fitInView(imgItem, Qt::KeepAspectRatio);

    QList<QRect> resultRecs = mReader->readImg(filename, translations);

    for (auto rect : resultRecs) {
        mPage->addItem(new TranslationRect{rect});
    }
}
