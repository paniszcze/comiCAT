#include "translationeditor.h"

TranslationEditor::TranslationEditor(QWidget *parent) :
    QWidget(parent),
    currSource(QModelIndex()),
    currTarget(QModelIndex()),
    x(0),
    y(0),
    width(0),
    height(0)
{
    setObjectName("TranslationEditor");
    setStyleSheet(
        "QWidget#TranslationEditor {border-radius: 4; border: 1 solid #e0e0e0;}"
        "QTextEdit {padding-left: 3px; padding-right: 3px;"
        "border: 1 solid #e0e0e0; border-radius: 4;"
        "background-color: white; color: black;"
        "selection-color: black; selection-background-color: #f2f2f2;"
        "font-size: 13px; font-weight: 300;}"
        "QLabel {font-size: 11px; color: #5c5c5c;}"
        "QLabel#Editor {padding-left: 1px; color: black; font-size: 13px;}"
        "QLabel#X, QLabel#Y, QLabel#Width, QLabel#Height"
        "{color: #828282; font-weight: 300;}"
        "QScrollBar:vertical {width: 8px; margin: 3px 1px 3px 1px;}"
        "QPushButton {font-size: 12px; color: #3c3c3c;}");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(createHeader());
    layout->addLayout(createEditor());
    layout->addLayout(createInfoBox());
    setLayout(layout);
    setEnabled(false);

    connect(sourceEdit,
            &QTextEdit::textChanged,
            this,
            &TranslationEditor::onTextChanged);
    connect(targetEdit,
            &QTextEdit::textChanged,
            this,
            &TranslationEditor::onTextChanged);
}

QHBoxLayout *TranslationEditor::createHeader()
{
    QLabel *title = new QLabel("Editor");
    title->setObjectName("Editor");

    QPushButton *translateBtn = new QPushButton("Translate", this);
    QPushButton *finishBtn = new QPushButton("Finish", this);

    QHBoxLayout *header = new QHBoxLayout();
    header->setSpacing(10);
    header->addWidget(title, 1, Qt::AlignLeft);
    header->addWidget(translateBtn);
    header->addWidget(finishBtn);

    return header;
}

QHBoxLayout *TranslationEditor::createEditor()
{
    sourceEdit = new QTextEdit(this);
    sourceEdit->setAcceptRichText(false);
    targetEdit = new QTextEdit(this);
    targetEdit->setAcceptRichText(false);

    QSizePolicy textEditSizePolicy{QSizePolicy::Ignored, QSizePolicy::Expanding};
    sourceEdit->setSizePolicy(textEditSizePolicy);
    targetEdit->setSizePolicy(textEditSizePolicy);

    QHBoxLayout *editor = new QHBoxLayout();
    editor->addWidget(sourceEdit);
    editor->addWidget(targetEdit);

    return editor;
}

QHBoxLayout *TranslationEditor::createLabel(QLabel *&label,
                                            int value,
                                            QString name,
                                            bool isLast)
{
    label = new QLabel(QString::number(value));
    label->setObjectName(name);

    QHBoxLayout *labelLayout = new QHBoxLayout();
    labelLayout->setSpacing(6);
    labelLayout->addWidget(new QLabel(name), 0, Qt::AlignRight);
    labelLayout->addWidget(label, isLast ? 1 : 0, Qt::AlignLeft);

    return labelLayout;
}

QHBoxLayout *TranslationEditor::createInfoBox()
{
    QHBoxLayout *infoBox = new QHBoxLayout();
    infoBox->setSpacing(20);
    infoBox->addLayout(createLabel(xLabel, x, "X", false));
    infoBox->addLayout(createLabel(yLabel, y, "Y", false));
    infoBox->addLayout(createLabel(widthLabel, width, "Width", false));
    infoBox->addLayout(createLabel(heightLabel, height, "Height", true));
    return infoBox;
}

void TranslationEditor::updateInfoBox()
{
    xLabel->setText(QString::number(x));
    yLabel->setText(QString::number(y));
    widthLabel->setText(QString::number(width));
    heightLabel->setText(QString::number(height));
}

void TranslationEditor::setInfoDetails(QRect rect)
{
    x = rect.left();
    y = rect.top();
    width = rect.width();
    height = rect.height();
}

void TranslationEditor::onSelectionChanged(const QItemSelection &selected,
                                           const QItemSelection &deselected)
{
    if (selected.isEmpty()) {
        if (deselected.isEmpty()) return;

        currSource = QModelIndex();
        currTarget = QModelIndex();
        sourceEdit->clear();
        targetEdit->clear();
        setInfoDetails(QRect());
        setEnabled(false);
    } else {
        setEnabled(true);
        currSource = selected.indexes()[0];
        currTarget = selected.indexes()[1];
        sourceEdit->setPlainText(currSource.data().toString());
        targetEdit->setPlainText(currTarget.data().toString());
        setInfoDetails(selected.indexes()[2].data().toRect());
    }

    updateInfoBox();
}

void TranslationEditor::onTextChanged()
{
    if (sender() == sourceEdit)
        emit itemNeedsUpdate(currSource, sourceEdit->toPlainText());
    else emit itemNeedsUpdate(currTarget, targetEdit->toPlainText());
}

void TranslationEditor::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
