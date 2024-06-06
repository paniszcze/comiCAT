#ifndef PAGE_H
#define PAGE_H

#include <QGraphicsScene>

class Page : public QGraphicsScene
{
public:
    explicit Page(QObject *parent = nullptr);

    void emitItemHoverd(QString name) { emit signalItemHovered(name); }

signals:
    void signalItemHovered(QString);
};

#endif // PAGE_H
