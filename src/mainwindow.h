#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QActionGroup>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMainWindow>
#include <QWidget>

#include "translationsmodel.h"
#include "translationsproxy.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setupActions();
    void setupToolBar();
    void setupStatusBar();
    void updateStatusBarVisibility();
    void setupTableView();

    void setEnabledEditorPane(bool enabled, bool completed = false);
    void clearEditorPane();
    void clearAndDisableEditorPane();
    void updateRectInfo(QRect rect);
    void updateProgress();

public slots:
    void openFile();
    void closeFile();

    void onCanvasZoomChanged(qreal percent);
    void onCanvasActionChanged();

    void onRowsInserted(const QModelIndex &parent, int first, int last);
    void onSelectionChanged(const QItemSelection &selected,
                            const QItemSelection &deselected);

    void onTextChanged();

    void onCompleteButtonClicked(bool checked);
    void onFilterChanged(QString filter);

public:
    TranslationsModel *translations;
    TranslationsProxy *proxy;

    QActionGroup *tools;
    QAction *actionSelect;
    QAction *actionDirectSelect;
    QAction *actionAdd;
    QAction *actionMerge;
    QAction *actionSplit;
    QAction *actionRemove;
    QAction *actionMove;
    QAction *actionZoom;
    QAction *actionSettings;

    QLabel *fileNameLabel;
    QLabel *zoomLabel;
    QLabel *pageLabel;
    QLabel *progressLabel;

private:
    Ui::MainWindow *ui;

    bool isFileOpened = false;
    QString currFilePath = "";
    QDir lastFileDialogDir;

    QAction *currTool = nullptr;
};
#endif // MAINWINDOW_H
