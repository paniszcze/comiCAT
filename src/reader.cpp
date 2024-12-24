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

QList<Translation> Reader::readImg(QString filename)
{
    Pix *image;
    QList<Translation> res;

    if ((image = pixRead(filename.toUtf8().constData()))) {
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
                    QRect bounds = {x1, y1, x2 - x1, y2 - y1};
                    res.append({bounds, text, ""});
                }
                delete[] cstring;
            } while (ri->Next(level));
        }
        pixDestroy(&image);
    }

    return res;
}
