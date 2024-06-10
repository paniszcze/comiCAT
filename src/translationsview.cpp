#include "translationsview.h"
#include "translationsviewstyle.h"

TranslationsView::TranslationsView(QWidget *parent)
    : QTableView(parent)
{
    setStyle(new TranslationsViewStyle(style()));
}
