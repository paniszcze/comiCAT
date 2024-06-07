#ifndef TRANSLATIONRECT_H
#define TRANSLATIONRECT_H

#include <QGraphicsRectItem>
#include <QPainter>
#include <QPen>
#include <QStyle>
#include <QStyleOptionGraphicsItem>

class TranslationRect : public QGraphicsRectItem
{
public:
    TranslationRect(const QRect &rect, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);
};

#endif // TRANSLATIONRECT_H
