TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += kernel \
           models \
           components \
           locale

TARGETPATH = MeeGo/Labs/Components

QML_FILES = qmldir

qmlfiles.files = $$QML_FILES
qmlfiles.sources = $$QML_FILES
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

INSTALLS += qmlfiles
