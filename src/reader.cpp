#include "reader.h"

Reader::Reader(QObject *parent)
    : QObject{parent}
{
    api = new tesseract::TessBaseAPI();
    if (api->Init("/usr/local/share/tessdata", "eng")) {
        qWarning() << "Couldn't initialise Tesseract";
        exit(1);
    }
    api->SetPageSegMode(tesseract::PageSegMode::PSM_AUTO);
}

Reader::~Reader()
{
    if (api != nullptr) {
        api->End();
        delete api;
    }
}

QList<QRect> Reader::readImg(QString filename, QStandardItemModel *translations)
{
    QList<QRect> resultRecs;

    Pix *image = pixRead(filename.toUtf8().constData());
    api->SetImage(image);
    api->Recognize(0);
    tesseract::ResultIterator *ri = api->GetIterator();
    tesseract::PageIteratorLevel level = tesseract::RIL_PARA;
    if (ri != 0) {
        do {
            const char *cstring = ri->GetUTF8Text(level);
            auto text = QString(cstring).trimmed();
            if (!text.isEmpty()) {
                int x1, y1, x2, y2;
                ri->BoundingBox(level, &x1, &y1, &x2, &y2);
                auto bounds = QRect(x1, y1, x2 - x1, y2 - y1);
                resultRecs.append(bounds);
                addTBox(translations, text, "", bounds);
            }
            delete[] cstring;
        } while (ri->Next(level));
    }
    pixDestroy(&image);

    return resultRecs;
}

void Reader::addTBox(QStandardItemModel *translations,
                     const QString &source,
                     const QString &target,
                     const QRect &bounds)
{
    int row = translations->rowCount();
    translations->insertRow(row);
    translations->setData(translations->index(row, 0), source);
    translations->setData(translations->index(row, 1), target);
    translations->setData(translations->index(row, 2), bounds);
}
