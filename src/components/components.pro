TEMPLATE = subdirs # XXX: Don't call the linker

TARGETPATH = MeeGo/Labs/Components

QML_FILES = qmldir \
            *.qml

qmlfiles.files = $$QML_FILES
qmlfiles.sources = $$QML_FILES
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

INSTALLS += qmlfiles
OBJECTS_DIR = .obj
MOC_DIR = .moc
