#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QRect>
#include <QString>

class Translation
{
public:
    Translation(QRect bounds = QRect(),
                QString sourceText = "",
                QString targetText = "");

    QRect bounds;
    QString sourceText;
    QString targetText;
};

#endif // TRANSLATION_H
