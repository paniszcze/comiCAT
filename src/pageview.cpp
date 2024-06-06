#include "pageview.h"

PageView::PageView(QWidget *parent)
    : QWidget{parent}
{
    mReader = new Reader();
    mPage = new QGraphicsScene();
    mPageView = new QGraphicsView(mPage);
    setStyleSheet("QGraphicsView {"
                  "border: none; outline: none;"
                  "background-color: white;"
                  "}");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mPageView);
}

PageView::~PageView()
{
    delete mReader;
}

void PageView::loadImg(QString filename, QStandardItemModel *translations)
{
    QPixmap img = QPixmap(filename);
    QGraphicsPixmapItem *imgItem = mPage->addPixmap(img);
    mPageView->fitInView(imgItem, Qt::KeepAspectRatio);
    QList<QRect> resultRecs = mReader->readImg(filename, translations);

    for (auto rect : resultRecs) {
        mPage->addRect(rect, QPen(Qt::magenta, 3));
    }
}
