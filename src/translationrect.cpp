#include "translationrect.h"

TranslationRect::TranslationRect(const QRect &rect,
                                 const QPersistentModelIndex &index,
                                 QGraphicsItem *parent) :
    QGraphicsRectItem(rect, parent), index(index)
{
    setPen(Qt::NoPen);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemIsSelectable);
}

QRectF TranslationRect::boundingRect() const
{
    QRectF boundingRect = QGraphicsRectItem::boundingRect();
    boundingRect.adjust(-PEN_WIDTH, -PEN_WIDTH, PEN_WIDTH, PEN_WIDTH);
    return boundingRect;
}

void TranslationRect::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (rect().isEmpty()) return;

    painter->setPen({ isSelected() ? Qt::green : Qt::magenta, PEN_WIDTH });
    painter->drawRect(
        QRectF(rect().x() - PEN_WIDTH / 2, rect().y() - PEN_WIDTH / 2,
               rect().width() + PEN_WIDTH, rect().height() + PEN_WIDTH));
}
