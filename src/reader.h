#ifndef READER_H
#define READER_H

#include <QDebug>
#include <QList>
#include <QObject>
#include <QRect>
#include <QStandardItemModel>
#include <QString>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <leptonica/pix_internal.h>

class Reader : public QObject
{
    Q_OBJECT

public:
    Reader(QObject *parent = nullptr);
    ~Reader();

    QList<QRect> readImg(QString filename, QStandardItemModel *translations);

    void addTBox(QStandardItemModel *translations,
                 const QString &source,
                 const QString &target,
                 const QRect &bounds);

private:
    tesseract::TessBaseAPI *api;
};

#endif // READER_H
