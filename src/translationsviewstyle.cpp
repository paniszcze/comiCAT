#include "translationsviewstyle.h"

TranslationsViewStyle::TranslationsViewStyle(QStyle* style)
    : QProxyStyle(style)
{}

void TranslationsViewStyle::drawPrimitive(PrimitiveElement element,
                                          const QStyleOption* option,
                                          QPainter* painter,
                                          const QWidget* widget) const
{
    if (element == QStyle::PE_IndicatorItemViewItemDrop
        && !option->rect.isNull())
    {
        QStyleOption opt(*option);
        opt.rect.setLeft(0);
        if (widget) { opt.rect.setRight(widget->width()); }
        painter->setPen(QColor(Qt::green));
        QProxyStyle::drawPrimitive(element, &opt, painter, widget);

        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
