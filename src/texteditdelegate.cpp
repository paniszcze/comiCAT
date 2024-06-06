#include "texteditdelegate.h"

TextEditDelegate::TextEditDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

QWidget *TextEditDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem & /* option */,
                                        const QModelIndex & /* index */) const
{
    QPlainTextEdit *editor = new QPlainTextEdit(parent);
    return editor;
}

void TextEditDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();

    QPlainTextEdit *textEdit = static_cast<QPlainTextEdit *>(editor);
    textEdit->setPlainText(text);
}

void TextEditDelegate::setModelData(QWidget *editor,
                                    QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    QPlainTextEdit *textEdit = static_cast<QPlainTextEdit *>(editor);
    QString text = textEdit->toPlainText();

    model->setData(index, text, Qt::EditRole);
}

void TextEditDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex & /* index */) const
{
    editor->setGeometry(option.rect);
}
