#-------------------------------------------------
#
# Project created by QtCreator 2015-06-29T16:43:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SourceAdminTool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    query.cpp \
    serverinfo.cpp \
    settings.cpp \
    worker.cpp \
    customitems.cpp \
    rcon.cpp \
    simplecrypt.cpp \
    main_events.cpp \
    main_slots.cpp \
    rcon_slots.cpp \
    query_slots.cpp \
    loghandler.cpp \
    loghandler_slots.cpp

HEADERS  += mainwindow.h \
    query.h \
    serverinfo.h \
    settings.h \
    worker.h \
    customitems.h \
    rcon.h \
    simplecrypt.h \
    ui_mainwindow.h \
    loghandler.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc

win32
{
    RC_FILE = sourceadmin.rc
    LIBS += -lws2_32
    LIBS += -lIPHlpApi
}
macx
{
    ICON = icons/icon.icns
}

unix:!macx|win32: LIBS += -L$$PWD/thirdparty/miniupnpc/libs/ -lminiupnpc

INCLUDEPATH += $$PWD/thirdparty/miniupnpc
DEPENDPATH += $$PWD/thirdparty/miniupnpc

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/thirdparty/miniupnpc/libs/miniupnpc.lib
else:unix:!macx|win32-g++: PRE_TARGETDEPS += $$PWD/thirdparty/miniupnpc/libs/libminiupnpc.a
