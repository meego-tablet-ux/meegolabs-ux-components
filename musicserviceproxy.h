/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MUSICSERVICEPROXY_H
#define MUSICSERVICEPROXY_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.meego.app.music
 */
class MusicServiceProxy: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.meego.app.music"; }

    MusicServiceProxy(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);
    ~MusicServiceProxy();

    Q_PROPERTY(QString state READ state NOTIFY stateChanged)
    inline QString state() const
    { return qvariant_cast< QString >(property("state")); }

Q_SIGNALS:
    void stateChanged();
};

#endif
