QT       += testlib

QT       -= gui

TARGET = tst_localunittesttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += qt \
          link_pkgconfig

OBJECTS_DIR = .obj
MOC_DIR = .moc

PKGCONFIG += mlite meegolocale

SOURCES +=  tst_meegolocale.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

unix:!macx:!symbian: LIBS += -L$$OUT_PWD/../../lib/locale/ -lmeegolocale

INCLUDEPATH += $$PWD/../../lib/locale
DEPENDPATH += $$PWD/../../lib/locale
