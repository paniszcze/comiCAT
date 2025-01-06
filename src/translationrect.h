#ifndef TRANSLATIONRECT_H
#define TRANSLATIONRECT_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPainter>
#include <QPen>
#include <QRect>
#include <QRectF>
#include <QStyle>
#include <QStyleOptionGraphicsItem>

static const qreal PEN_WIDTH = 2;

class TranslationRect : public QGraphicsRectItem
{
public:
    TranslationRect(const QRect &rect, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
};

#endif // TRANSLATIONRECT_H
