TEMPLATE = lib

TARGETPATH = MeeGo/Labs/Locale

TARGET = meegolabs-ux-locale

QT += declarative

CONFIG += qt \
          link_pkgconfig \
          plugin

INCLUDEPATH += ../../lib/locale
DEFINES += LOCALE_PLUGIN

PKGCONFIG += icu-i18n icu-uc mlite

SOURCES += plugin.cpp \
           ../../lib/locale/meegolocale.cpp \
           ../../lib/locale/localelistmodel.cpp

HEADERS += plugin.h \
           ../../lib/locale/meegolocale.h \
           ../../lib/locale/meegolocale_global.h \
           ../../lib/locale/localelistmodel.h

QML_FILES = qmldir

qmlfiles.files = $$QML_FILES
qmlfiles.sources = $$QML_FILES
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

INSTALLS += target qmlfiles
OBJECTS_DIR = .obj
MOC_DIR = .moc
