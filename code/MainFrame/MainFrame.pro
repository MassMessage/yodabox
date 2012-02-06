#-------------------------------------------------
#
# Project created by QtCreator 2011-12-03T22:24:18
#
#-------------------------------------------------

QT       += core gui network webkit

TARGET = ../../BIN/YodaBox
TEMPLATE = app



CONFIG  += qxt
QXT     += core


LIBS += -L"F:\Tech\QT\depoly" -L../BIN -lQxtCore -lQJson

DEPENDPATH += "F:\Tech\QT\depoly"
INCLUDEPATH += "../"
INCLUDEPATH += "../include/"
INCLUDEPATH += "F:\Tech\QT\libqxt-libqxt-v0.6.2\src\core"
INCLUDEPATH += "D:/boost/boost_1_48_0"


SOURCES += main.cpp\        
    loadthread.cpp


HEADERS  += loadthread.h \






win32 {



    DEFINES = VC_EXTRALEAN \
        WIN32 \
        _UNICODE \
        UNICODE \
        WINVER=0x0600 \
        _WIN32_WINNT=0x0600 \
        _WIN32_WINDOWS=0x0600 \
        _WIN32_IE=0x0700
}





win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../yodacommon/release/ -lyodacommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../yodacommon/debug/ -lyodacommon
else:symbian: LIBS += -lyodacommon
else:unix: LIBS += -L$$OUT_PWD/../yodacommon/ -lyodacommon

INCLUDEPATH += $$PWD/../yodacommon
DEPENDPATH += $$PWD/../yodacommon




win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../yodaframework/release/ -lyodaframework
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../yodaframework/debug/ -lyodaframework
else:symbian: LIBS += -lyodaframework
else:unix: LIBS += -L$$OUT_PWD/../yodaframework/ -lyodaframework

INCLUDEPATH += $$PWD/../yodaframework
DEPENDPATH += $$PWD/../yodaframework




win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MainModule/release/ -lMainModule
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MainModule/debug/ -lMainModule
else:symbian: LIBS += -lMainModule
else:unix: LIBS += -L$$OUT_PWD/../MainModule/ -lMainModule

INCLUDEPATH += $$PWD/../MainModule
DEPENDPATH += $$PWD/../MainModule
