#ifndef TRANSLATIONEDITOR_H
#define TRANSLATIONEDITOR_H

#include <QHBoxLayout>
#include <QItemSelection>
#include <QLabel>
#include <QModelIndex>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class TranslationEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TranslationEditor(QWidget *parent = nullptr);

public slots:
    void onSelectionChanged(const QItemSelection &selected,
                            const QItemSelection &deselected);

private:
    QHBoxLayout *createHeader();
    QHBoxLayout *createEditor();
    QHBoxLayout *createInfoBox();
    QHBoxLayout *createLabel(QLabel *&label,
                             int value,
                             QString name,
                             bool isLast);

    void setInfoDetails(QRect rect);
    void updateInfoBox();

    void paintEvent(QPaintEvent *) override;

public:
    QTextEdit *sourceEdit;
    QTextEdit *targetEdit;
    QLabel *xLabel;
    QLabel *yLabel;
    QLabel *widthLabel;
    QLabel *heightLabel;

private:
    int x, y, width, height;
};

#endif // TRANSLATIONEDITOR_H
