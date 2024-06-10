#ifndef TRANSLATIONSVIEWSTYLE_H
#define TRANSLATIONSVIEWSTYLE_H

#include <QObject>
#include <QPainter>
#include <QProxyStyle>
#include <QStyle>
#include <QStyleOption>
#include <QWidget>

class TranslationsViewStyle : public QProxyStyle
{
    Q_OBJECT
public:
    TranslationsViewStyle(QStyle* style = nullptr);

    void drawPrimitive(PrimitiveElement element,
                       const QStyleOption* option,
                       QPainter* painter,
                       const QWidget* widget = nullptr) const;
};

#endif // TRANSLATIONSVIEWSTYLE_H
