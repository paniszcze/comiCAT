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
