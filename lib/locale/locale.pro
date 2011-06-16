TEMPLATE = lib

TARGET = $$qtLibraryTarget(ux-localehelper)

DEFINES += LOCALE_LIBRARY

CONFIG += qt

OBJECTS_DIR = .obj
MOC_DIR = .moc

SOURCES +=  localehelper.cpp

HEADERS +=  locale_global.h \
            localehelper.h

INSTALL_HEADERS += locale_global.h \
                   localehelper.h

target.path += $$[QT_INSTALL_LIBS]

headers.files += $$INSTALL_HEADERS
headers.path += $$INSTALL_ROOT/usr/include/ux-localehelper

INSTALLS += target headers

TRANSLATIONS += $${SOURCES} $${HEADERS} $${OTHER_FILES}

PROJECT_NAME = localehelper
