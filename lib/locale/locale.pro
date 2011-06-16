TEMPLATE = lib
TARGET = $$qtLibraryTarget(meegolocale)
PROJECT_NAME = meegolocale

DEFINES += LOCALE_LIBRARY

CONFIG += qt

OBJECTS_DIR = .obj
MOC_DIR = .moc

SOURCES +=  meegolocale.cpp

HEADERS +=  meegolocale_global.h \
            meegolocale.h

INSTALL_HEADERS += meegolocale_global.h \
                   meegolocale.h

target.path += $$[QT_INSTALL_LIBS]

headers.files += $$INSTALL_HEADERS
headers.path += $$INSTALL_ROOT/usr/include

INSTALLS += target headers

TRANSLATIONS += $${SOURCES} $${HEADERS} $${OTHER_FILES}
