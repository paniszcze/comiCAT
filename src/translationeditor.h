#ifndef TRANSLATIONEDITOR_H
#define TRANSLATIONEDITOR_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QWidget>

class TranslationEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TranslationEditor(QWidget *parent = nullptr);

signals:

private:
    void paintEvent(QPaintEvent *) override;
};

#endif // TRANSLATIONEDITOR_H
