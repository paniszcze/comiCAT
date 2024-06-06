#ifndef TRANSLATIONRECT_H
#define TRANSLATIONRECT_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>

class TranslationRect : public QGraphicsRectItem
{
public:
    TranslationRect();

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

    void activate();
    void deactivate();

private:
    QGraphicsScene *scene;
};

#endif // TRANSLATIONRECT_H
