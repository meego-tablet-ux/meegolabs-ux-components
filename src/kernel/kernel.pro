TEMPLATE = lib

TARGETPATH = MeeGo/Labs/Kernel

TARGET = meegolabs-ux-kernel

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

SOURCES +=  desktop.cpp \
            desktopdatabase.cpp \
            gconfitem.cpp \
            imageextension.cpp \
            meegothemedimageprovider.cpp \
            paintspy.cpp \
            qmldebugtools.cpp \
            relativetimestamp.cpp \
            roundeditem.cpp \
            speechbubbledialog.cpp \
            stricturl.cpp \
            systemiconprovider.cpp \
            timestampupdater.cpp \
            translator.cpp \
            plugin.cpp \
            udisk_interface.cpp \
            valuespacepublisher.cpp \
            valuespacesubscriber.cpp \
            windowelement.cpp \
            windowiconprovider.cpp \
            windowlistener.cpp

HEADERS +=  desktop.h \
            desktopdatabase.h \
            gconfitem.h \
            imageextension.h \
            meegothemedimageprovider.h \
            paintspy.h \
            qmldebugtools.h \
            relativetimestamp.h \
            roundeditem.h \
            speechbubbledialog.h \
            stricturl.h \
            systemiconprovider.h \
            timestampupdater.h \
            translator.h \
            plugin.h \
            udisk_interface.h \
            valuespacepublisher.h \
            valuespacesubscriber.h \
            windowelement.h \
            windowiconprovider.h \
            windowlistener.h \
            windowinfo.h

QML_FILES = qmldir

qmlfiles.files = $$QML_FILES
qmlfiles.sources = $$QML_FILES
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

INSTALLS += target qmlfiles
OBJECTS_DIR = .obj
MOC_DIR = .moc
