TEMPLATE = lib

TARGETPATH = MeeGo/Labs/Locale

TARGET = meegolabs-ux-locale

DEFINES += LOCALE_LIBRARY

QT += declarative

CONFIG += qt \
          plugin

SOURCES +=  localehelper.cpp \
            plugin.cpp

HEADERS +=  locale_global.h \
            localehelper.h \
            plugin.h

INSTALL_HEADERS += localehelper.h

QML_FILES = qmldir

qmlfiles.files = $$QML_FILES
qmlfiles.sources = $$QML_FILES
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

lib.files += $$[QT_INSTALL_IMPORTS]/$$TARGETPATH/*.so
lib.path  += $$INSTALL_ROOT/usr/lib/ux-localehelper

headers.files += $$INSTALL_HEADERS
headers.path += $$INSTALL_ROOT/usr/include/ux-localehelper

INSTALLS += target qmlfiles headers lib
OBJECTS_DIR = .obj
MOC_DIR = .moc
