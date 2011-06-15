TEMPLATE = lib

TARGETPATH = MeeGo/Labs/Locale

TARGET = meegolabs-ux-locale

QT += declarative

CONFIG += qt \
          plugin

DEFINES += LOCALE_PLUGIN

SOURCES += plugin.cpp \
           ../../lib/locale/localehelper.cpp

HEADERS += plugin.h \
           ../../lib/locale/localehelper.h

QML_FILES = qmldir

qmlfiles.files = $$QML_FILES
qmlfiles.sources = $$QML_FILES
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

INSTALLS += target qmlfiles
OBJECTS_DIR = .obj
MOC_DIR = .moc
