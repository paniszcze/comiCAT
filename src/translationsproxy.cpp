#include "translationsproxy.h"
#include "translationsmodel.h"

TranslationsProxy::TranslationsProxy() {}

bool TranslationsProxy::filterAcceptsRow(int sourceRow,
                                         const QModelIndex &sourceParent) const
{
    QModelIndex completeIndex = sourceModel()->index(sourceRow, COMPLETED,
                                                     sourceParent);
    bool completed = sourceModel()->data(completeIndex).toBool();

    if (filter == "Completed") return completed;
    if (filter == "Working") return !completed;

    return true;
}
