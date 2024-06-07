#include "translationrect.h"

TranslationRect::TranslationRect(const QRect &rect, QGraphicsItem *parent)
    : QGraphicsRectItem(rect, parent)
{
    setPen(Qt::NoPen);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemIsSelectable);
}

void TranslationRect::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen({isSelected() ? Qt::green : Qt::magenta, 3});
    painter->drawRect(boundingRect());
}
