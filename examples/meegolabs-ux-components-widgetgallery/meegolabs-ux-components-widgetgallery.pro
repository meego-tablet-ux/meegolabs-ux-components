TEMPLATE = subdirs
TARGET = meegolabs-ux-components-widgetgallery

qmlfiles.files += *.qml images/
qmlfiles.path += $$INSTALL_ROOT/usr/share/$$TARGET

INSTALLS += qmlfiles

QML_FILES += *.qml \
             *.js

OTHER_FILES += $${QML_FILES}

TRANSLATIONS += $${QML_FILES}

PROJECT_NAME = meegolabs-ux-components-widgetgallery
