QT       += core
QT       += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

LIBS += \
    -L/opt/homebrew/Cellar/tesseract/5.5.0/lib -ltesseract \
    -L/opt/homebrew/Cellar/libarchive/3.7.7/lib -larchive \
    -L/opt/homebrew/Cellar/leptonica/1.85.0/lib -lleptonica \
    -lcurl

INCLUDEPATH += \
    ./src \
    /opt/homebrew/Cellar/tesseract/5.5.0/include \
    /opt/homebrew/Cellar/leptonica/1.85.0/include

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/pageview.cpp \
    src/reader.cpp \
    src/translationrect.cpp \
    src/translationsmodel.cpp \
    src/translationsproxy.cpp \
    src/translationsview.cpp

HEADERS += \
    src/mainwindow.h \
    src/pageview.h \
    src/reader.h \
    src/translationrect.h \
    src/translationsmodel.h \
    src/translationsproxy.h \
    src/translationsview.h

FORMS += \
    src/mainwindow.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
