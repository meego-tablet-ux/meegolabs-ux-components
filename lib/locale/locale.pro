TEMPLATE = lib

TARGETPATH = /usr/lib/ux-localehelper

TARGET = ux-localehelper

DEFINES += LOCALE_LIBRARY

CONFIG += qt

SOURCES +=  localehelper.cpp

HEADERS +=  locale_global.h \
            localehelper.h

INSTALL_HEADERS += locale_global.h \
                   localehelper.h

target.files += *.so
target.path += $$TARGETPATH

headers.files += $$INSTALL_HEADERS
headers.path += $$INSTALL_ROOT/usr/include/ux-localehelper

INSTALLS += target headers
OBJECTS_DIR = .obj
MOC_DIR = .moc
