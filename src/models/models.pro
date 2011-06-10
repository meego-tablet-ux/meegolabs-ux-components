TEMPLATE = lib

TARGETPATH = MeeGo/Labs/Models

TARGET = meegolabs-ux-models

QT += declarative \
    network \
    dbus \
    sql
CONFIG += qt \
    plugin \
    dbus \
    link_pkgconfig \
    mobility
PKGCONFIG += gconf-2.0 \
    qmfmessageserver \
    qmfclient \
    libpulse \
    libpulse-mainloop-glib \
    libexif \
    libkcalcoren \
    mlite \
    xdamage \
    QtPublishSubscribe \
    contextsubscriber-1.0

SOURCES +=  applicationsmodel.cpp \
	    appupappsmodel.cpp \
	    backgroundmodel.cpp \
	    browserlistmodel.cpp \
	    browserserviceiface.cpp \
	    devicemodel.cpp \
	    favoriteapplicationsmodel.cpp \
	    librarymodel.cpp \
	    musicserviceproxy.cpp \
	    recentapplicationsmodel.cpp \
	    timezonelistmodel.cpp \
	    udisk_interface.cpp \
	    windowmodel.cpp \
    plugin.cpp

HEADERS +=  applicationsmodel.h \
	    appupappsmodel.h \
	    backgroundmodel.h \
	    browserlistmodel.h \
	    browserserviceiface.h \
	    devicemodel.h \
	    favoriteapplicationsmodel.h \
	    librarymodel.h \
	    musicserviceproxy.h \
	    recentapplicationsmodel.h \
	    timezonelistmodel.h \
	    udisk_interface.h \
	    windowmodel.h \
    plugin.h

QML_FILES = qmldir

qmlfiles.files = $$QML_FILES
qmlfiles.sources = $$QML_FILES
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

INSTALLS += target qmlfiles
OBJECTS_DIR = .obj
MOC_DIR = .moc
