QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

LIBS += \
    -L/opt/homebrew/Cellar/tesseract/5.3.4_1/lib -ltesseract \
    -L/opt/homebrew/Cellar/libarchive/3.7.4/lib -larchive \
    -L/opt/homebrew/Cellar/leptonica/1.84.1/lib -lleptonica \
    -lcurl

INCLUDEPATH += \
    /opt/homebrew/Cellar/tesseract/5.3.4_1/include \
    /opt/homebrew/Cellar/leptonica/1.84.1/include

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
