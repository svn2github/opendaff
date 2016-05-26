#-------------------------------------------------
#
# Project created by QtCreator 2016-05-20T17:54:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DAFFViewer
TEMPLATE = app


SOURCES += src/main.cpp \
    src/mainwindowdaffviewer.cpp

HEADERS  += \
    src/mainwindowdaffviewer.h

FORMS    += \
    ui/mainwindowdaffviewer.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../win32-x64.vc12/lib/ -lDAFF -lDAFFViz
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../win32-x64.vc12/lib/ -lDAFFD -lDAFFVizD
else:unix: LIBS += -L$$PWD/../../../win32-x64.vc12/lib/ -lDAFF

INCLUDEPATH += $$PWD/../../../win32-x64.vc12/include
DEPENDPATH += $$PWD/../../../win32-x64.vc12/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../win32-x64.vc12/lib/libDAFF.a $$PWD/../../../win32-x64.vc12/lib/libDAFFViz.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../win32-x64.vc12/lib/libDAFFD.a  $$PWD/../../../win32-x64.vc12/lib/libDAFFVizDa
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../win32-x64.vc12/lib/DAFF.lib $$PWD/../../../win32-x64.vc12/lib/DAFFViz.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../win32-x64.vc12/lib/DAFFd.lib $$PWD/../../../win32-x64.vc12/lib/DAFFVizD.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../win32-x64.vc12/lib/libDAFF.a $$PWD/../../../win32-x64.vc12/lib/libDAFFViz.a
