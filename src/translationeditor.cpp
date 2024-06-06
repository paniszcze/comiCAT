#include "translationeditor.h"

TranslationEditor::TranslationEditor(QWidget *parent)
    : QWidget{parent}
{
    setObjectName("TranslationEditor");
    setStyleSheet(
        "QWidget#TranslationEditor {border-radius: 4; border: 1 solid #e0e0e0;}"
        "QPlainTextEdit {background-color: white; color: black;"
        "border: 1 solid #e0e0e0; border-radius: 4; padding: 5px;"
        "font-size: 13px;}"
        "QLabel {font-size: 11px; color: #5c5c5c;}"
        "QLabel#Editor {font-size: 13px; color: black; padding-left: 1px;}"
        "QPushButton {font-size: 12px; color: #3c3c3c;}");

    QLabel *title = new QLabel("Editor");
    title->setObjectName("Editor");
    QPushButton *translateBtn = new QPushButton("Translate", this);
    QPushButton *finishBtn = new QPushButton("Finish", this);

    QHBoxLayout *header = new QHBoxLayout();
    header->setSpacing(10);
    header->addWidget(title, 1, Qt::AlignLeft);
    header->addWidget(translateBtn);
    header->addWidget(finishBtn);

    QPlainTextEdit *sourceEdit = new QPlainTextEdit(this);
    QPlainTextEdit *targetEdit = new QPlainTextEdit(this);

    QSizePolicy textEditSizePolicy{QSizePolicy::Ignored, QSizePolicy::Preferred};
    sourceEdit->setSizePolicy(textEditSizePolicy);
    targetEdit->setSizePolicy(textEditSizePolicy);

    QHBoxLayout *editor = new QHBoxLayout();
    editor->addWidget(sourceEdit);
    editor->addWidget(targetEdit);

    QLabel *xLabel = new QLabel("X");
    QLabel *xVal = new QLabel("1145");
    QLabel *yLabel = new QLabel("Y");
    QLabel *yVal = new QLabel("260");
    QLabel *widthLabel = new QLabel("Width");
    QLabel *widthVal = new QLabel("607");
    QLabel *heightLabel = new QLabel("Height");
    QLabel *heightVal = new QLabel("29");

    QHBoxLayout *tboxInfo = new QHBoxLayout();
    tboxInfo->setSpacing(10);
    tboxInfo->addWidget(xLabel, 0, Qt::AlignRight);
    tboxInfo->addWidget(xVal, 0, Qt::AlignLeft);
    tboxInfo->addWidget(yLabel, 0, Qt::AlignRight);
    tboxInfo->addWidget(yVal, 0, Qt::AlignLeft);
    tboxInfo->addWidget(widthLabel, 0, Qt::AlignRight);
    tboxInfo->addWidget(widthVal, 0, Qt::AlignLeft);
    tboxInfo->addWidget(heightLabel, 0, Qt::AlignRight);
    tboxInfo->addWidget(heightVal, 1, Qt::AlignLeft);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(header);
    layout->addLayout(editor);
    layout->addLayout(tboxInfo);
    setLayout(layout);
}

void TranslationEditor::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
