TEMPLATE = lib
TARGET = Components
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
MOBILITY = publishsubscribe
TARGET = $$qtLibraryTarget($$TARGET)
DESTDIR = $$TARGET
OBJECTS_DIR = .obj
MOC_DIR = .moc

# Input
SOURCES += \
    applicationsmodel.cpp \
    backgroundmodel.cpp \
    batteryindicator.cpp \
    bluetoothindicator.cpp \
    browserlistmodel.cpp \
    browserserviceiface.cpp \
    components.cpp \
    desktop.cpp \
    desktopdatabase.cpp \
    devicemodel.cpp \
    favoriteapplicationsmodel.cpp \
    fuzzydatetime.cpp \
    gconfitem.cpp \
    gesturearea.cpp \
    imageextension.cpp \
    librarymodel.cpp \
    localehelper.cpp \
    localtime.cpp \
    musicindicator.cpp \
    musicserviceproxy.cpp \
    networkindicator.cpp \
    notificationindicator.cpp \
    paintspy.cpp \
    qmldebugtools.cpp \
    recentapplicationsmodel.cpp \
    roundeditem.cpp \
    speechbubbledialog.cpp \
    stricturl.cpp \
    systemiconprovider.cpp \
    themedimageprovider.cpp \
    meegothemedimageprovider.cpp \
    timezonelistmodel.cpp\
    translator.cpp \
    udisk_interface.cpp \
    valuespacepublisher.cpp \
    valuespacesubscriber.cpp \
    volumecontrol.cpp \
    windowelement.cpp \
    windowiconprovider.cpp \
    windowlistener.cpp \
    windowmodel.cpp

QML_SOURCES += \
    Components/*.qml

OTHER_FILES += \
    $${QML_SOURCES} \
    Components/qmldir

HEADERS += \
    applicationsmodel.h \
    backgroundmodel.h \
    batteryindicator.h \
    bluetoothindicator.h \
    browserlistmodel.h \
    browserserviceiface.h \
    components.h \
    desktop.h \
    desktopdatabase.h \
    devicemodel.h \
    favoriteapplicationsmodel.h \
    fuzzydatetime.h \
    gconfitem.h \
    gesturearea.h \
    imageextension.h \
    librarymodel.h \
    localehelper.h \
    localtime.h \
    meegothemedimageprovider.h \
    musicindicator.h \
    musicserviceproxy.h \
    networkindicator.h \
    notificationindicator.h \
    paintspy.h \
    qmldebugtools.h \
    recentapplicationsmodel.h \
    roundeditem.h \
    speechbubbledialog.h \
    stricturl.h \
    systemiconprovider.h \
    timezonelistmodel.h \
    translator.h \
    udisk_interface.h \
    valuespacepublisher.h \
    valuespacesubscriber.h \
    volumecontrol.h \
    windowelement.h \
    windowiconprovider.h \
    windowinfo.h \
    windowlistener.h \
    windowmodel.h

qmldir.files += $$TARGET
qmldir.path += $$[QT_INSTALL_IMPORTS]/MeeGo/Labs/
INSTALLS += qmldir

TRANSLATIONS += $${SOURCES} $${HEADERS} $${QML_SOURCES}
VERSION = 0.2.6.4
PROJECT_NAME = meegolabs-ux-components

dist.commands += rm -fR $${PROJECT_NAME}-$${VERSION} &&
dist.commands += git clone . $${PROJECT_NAME}-$${VERSION} &&
dist.commands += rm -fR $${PROJECT_NAME}-$${VERSION}/.git &&
dist.commands += mkdir -p $${PROJECT_NAME}-$${VERSION}/ts &&
dist.commands += lupdate $${TRANSLATIONS} -ts $${PROJECT_NAME}-$${VERSION}/ts/$${PROJECT_NAME}.ts &&
dist.commands += tar jcpvf $${PROJECT_NAME}-$${VERSION}.tar.bz2 $${PROJECT_NAME}-$${VERSION}
QMAKE_EXTRA_TARGETS += dist
