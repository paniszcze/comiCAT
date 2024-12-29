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
#include <QMenuBar>
#include <QSizePolicy>
#include <QStatusBar>
#include <QToolBar>
#include <QWidget>

#include "reader.h"
#include "translationsmodel.h"

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

private:
    void setInfoDetails(QRect rect);
    void updateInfoBox();

public slots:
    void openFile();
    void closeFile();
    void onCanvasZoomChanged(qreal percent);
    void onCanvasActionChanged();
    void onSelectionChanged(const QItemSelection &selected,
                            const QItemSelection &deselected);
    void onTextChanged();
    void onCompleteButtonClicked(bool checked);

public:
    TranslationsModel *translations;
    Reader *reader;

    QActionGroup *canvasActions;
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

    int x, y, width, height;

private:
    Ui::MainWindow *ui;

    bool isFileOpened;
    QString currFilePath;
    QDir lastFileDialogDir;

    QAction *currCanvasAction;

    QModelIndex currSource;
    QModelIndex currTarget;
};
#endif // MAINWINDOW_H
