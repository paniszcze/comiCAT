#include "mainwindow.h"

#include <QApplication>
#include <QPainter>
#include <QProxyStyle>
#include <QStyleOption>

class Style : public QProxyStyle
{
public:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const
    {
        // Do not draw focus rectangles
        if (element == QStyle::PE_FrameFocusRect) return;

        // Paint drop indicator green across the entire widget width
        if (element == QStyle::PE_IndicatorItemViewItemDrop
            && !option->rect.isNull())
        {
            QStyleOption opt(*option);
            opt.rect.setLeft(0);
            if (widget) opt.rect.setRight(widget->width());
            painter->setPen(QColor(Qt::green));
            QProxyStyle::drawPrimitive(element, &opt, painter, widget);
            return;
        }

        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(new Style);
    MainWindow w;
    w.show();
    return a.exec();
}
