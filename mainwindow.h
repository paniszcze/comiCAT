#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <leptonica/pix_internal.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
