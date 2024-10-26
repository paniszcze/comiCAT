QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += gui

CONFIG += c++17

LIBS += \
    -L/opt/homebrew/Cellar/tesseract/5.3.4_1/lib -ltesseract \
    -L/opt/homebrew/Cellar/libarchive/3.7.4/lib -larchive \
    -L/opt/homebrew/Cellar/leptonica/1.84.1/lib -lleptonica \
    -lcurl

INCLUDEPATH += \
    ./src \
    /opt/homebrew/Cellar/tesseract/5.3.4_1/include \
    /opt/homebrew/Cellar/leptonica/1.84.1/include

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/pageview.cpp \
    src/reader.cpp \
    src/translation.cpp \
    src/translationeditor.cpp \
    src/translationlist.cpp \
    src/translationrect.cpp \
    src/translationsmodel.cpp \
    src/translationsview.cpp \
    src/translationsviewstyle.cpp

HEADERS += \
    src/mainwindow.h \
    src/pageview.h \
    src/reader.h \
    src/translation.h \
    src/translationeditor.h \
    src/translationlist.h \
    src/translationrect.h \
    src/translationsmodel.h \
    src/translationsview.h \
    src/translationsviewstyle.h

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
