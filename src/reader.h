#ifndef READER_H
#define READER_H

#include <QDebug>
#include <QList>
#include <QObject>
#include <QRect>
#include <QString>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <leptonica/pix_internal.h>

#include "translation.h"

class Reader : public QObject
{
    Q_OBJECT

public:
    Reader(QObject *parent = nullptr);
    ~Reader();
    QList<Translation> readImg(QString filename);

private:
    tesseract::TessBaseAPI *api;
};

#endif // READER_H
