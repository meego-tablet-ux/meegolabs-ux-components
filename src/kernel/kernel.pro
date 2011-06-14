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

SOURCES +=  \
  applicationsmodel.cpp \
  appupappsmodel.cpp \
  backgroundmodel.cpp \
  browserlistmodel.cpp \
  browserserviceiface.cpp \
  desktop.cpp \
  desktopdatabase.cpp \
  devicemodel.cpp \
  favoriteapplicationsmodel.cpp \
  gconfitem.cpp \
  imageextension.cpp \
  librarymodel.cpp \
  meegothemedimageprovider.cpp \
  musicserviceproxy.cpp \
  paintspy.cpp \
  plugin.cpp \
  qmldebugtools.cpp \
  recentapplicationsmodel.cpp \
  relativetimestamp.cpp \
  roundeditem.cpp \
  speechbubbledialog.cpp \
  stricturl.cpp \
  systemiconprovider.cpp \
  timestampupdater.cpp \
  themedimageprovider.cpp \
  timezonelistmodel.cpp \
  translator.cpp \
  udisk_interface.cpp \
  valuespacepublisher.cpp \
  valuespacesubscriber.cpp \
  windowelement.cpp \
  windowiconprovider.cpp \
  windowlistener.cpp \
  windowmodel.cpp

HEADERS += \
  applicationsmodel.h \
  appupappsmodel.h \
  backgroundmodel.h \
  browserlistmodel.h \
  browserserviceiface.h \
  desktop.h \
  desktopdatabase.h \
  devicemodel.h \
  favoriteapplicationsmodel.h \
  gconfitem.h \
  imageextension.h \
  librarymodel.h \
  meegothemedimageprovider.h \
  musicserviceproxy.h \
  paintspy.h \
  plugin.h \
  qmldebugtools.h \
  recentapplicationsmodel.h \
  relativetimestamp.h \
  roundeditem.h \
  speechbubbledialog.h \
  stricturl.h \
  systemiconprovider.h \
  timestampupdater.h \
  timezonelistmodel.h \
  translator.h \
  themedimageprovider.h \
  udisk_interface.h \
  valuespacepublisher.h \
  valuespacesubscriber.h \
  windowelement.h \
  windowiconprovider.h \
  windowinfo.h \
  windowlistener.h \
  windowmodel.h

QML_FILES = qmldir

qmlfiles.files = $$QML_FILES
qmlfiles.sources = $$QML_FILES
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

INSTALLS += target qmlfiles
OBJECTS_DIR = .obj
MOC_DIR = .moc
