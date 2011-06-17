TEMPLATE = lib
TARGET = $$qtLibraryTarget(meegolocale)
PROJECT_NAME = meegolocale

DEFINES += LOCALE_LIBRARY

CONFIG += qt \
          link_pkgconfig

OBJECTS_DIR = .obj
MOC_DIR = .moc

PKGCONFIG += icu-i18n icu-uc

SOURCES +=  meegolocale.cpp \
            localelistmodel.cpp

HEADERS +=  meegolocale_global.h \
            meegolocale.h \
            localelistmodel.h

INSTALL_HEADERS += meegolocale_global.h \
                   meegolocale.h

target.path += $$[QT_INSTALL_LIBS]

headers.files += $$INSTALL_HEADERS
headers.path += $$INSTALL_ROOT/usr/include

INSTALLS += target headers

EXTRA_CLEAN += $${OBJECTS_DIR}/* $${MOC_DIR}/*

TRANSLATIONS += $${SOURCES} $${HEADERS} $${OTHER_FILES}
