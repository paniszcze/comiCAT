#include "translationrect.h"

TranslationRect::TranslationRect()
{
    setAcceptHoverEvents(true);
}

void TranslationRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    // scene->EmitItemHoverd(itemName);
}

void TranslationRect::activate()
{
    setPen({Qt::green, 3});
}

void TranslationRect::deactivate()
{
    setPen({Qt::magenta, 3});
}
