TEMPLATE = lib
PROJECT_NAME = meegolocale
TARGET = $$qtLibraryTarget($$PROJECT_NAME)

DEFINES += LOCALE_LIBRARY

CONFIG += qt \
          link_pkgconfig

OBJECTS_DIR = .obj
MOC_DIR = .moc

PKGCONFIG += icu-i18n icu-uc mlite

SOURCES +=  meegolocale.cpp \
            localelistmodel.cpp

HEADERS +=  meegolocale_global.h \
            meegolocale.h \
            localelistmodel.h

INSTALL_HEADERS += meegolocale_global.h \
                   meegolocale.h \
                   localelistmodel.h

target.path += $$[QT_INSTALL_LIBS]

headers.files += $$INSTALL_HEADERS
headers.path += $$INSTALL_ROOT/usr/include

pcfiles.files += $${PROJECT_NAME}.pc
pcfiles.path += $$INSTALL_ROOT/usr/lib/pkgconfig

INSTALLS += target headers pcfiles

EXTRA_CLEAN += $${OBJECTS_DIR}/* $${MOC_DIR}/*

TRANSLATIONS += $${SOURCES} $${HEADERS} $${OTHER_FILES}
