#-------------------------------------------------
#
# Project created by QtCreator 2010-11-22T11:07:57
#
#-------------------------------------------------

QT       += svg xml

TARGET = Iso8583extract
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qskinobject.cpp \
    qskinwidgetresizehandler.cpp \
    isoextract.cpp \
    isofield.cpp

HEADERS  += mainwindow.h \
    qskinobject.h \
    qskinwidgetresizehandler.h \
    isoextract.h \
    isofield.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc

win32 {
    RC_FILE = iso8583extract.rc
}
