TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += src \
           examples

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


TRANSLATIONS += src
VERSION = 0.2.8.6
PROJECT_NAME = meegolabs-ux-components

dist.commands += rm -fR $${PROJECT_NAME}-$${VERSION} &&
dist.commands += [ ! -e .git/refs/tags/$${VERSION} ] || git tag -d $${VERSION} &&
dist.commands += git tag $${VERSION} -m \"make dist auto-tag for $${VERSION}\" &&
dist.commands += git archive --prefix=$${PROJECT_NAME}-$${VERSION}/ $${VERSION} --format=tar | tar x &&
dist.commands += mkdir -p $${PROJECT_NAME}-$${VERSION}/ts &&
dist.commands += lupdate $${TRANSLATIONS} -ts $${PROJECT_NAME}-$${VERSION}/ts/$${PROJECT_NAME}.ts &&
dist.commands += tar jcpvf $${PROJECT_NAME}-$${VERSION}.tar.bz2 $${PROJECT_NAME}-$${VERSION}
QMAKE_EXTRA_TARGETS += dist

