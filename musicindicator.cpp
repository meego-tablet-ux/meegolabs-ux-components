/*and a
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "musicindicator.h"
#include "musicserviceproxy.h"

#define SERVICE "com.meego.app.music"

#include <QDBusReply>
#include <QDBusConnectionInterface>

MusicIndicator::MusicIndicator(QObject *parent) : QObject(parent) , m_innerMI(0)
{

    m_serviceWatcher = new QDBusServiceWatcher(SERVICE,
                                               QDBusConnection::sessionBus(),
                                               QDBusServiceWatcher::WatchForRegistration |
                                               QDBusServiceWatcher::WatchForUnregistration,
                                               this);
    connect(m_serviceWatcher, SIGNAL(serviceRegistered(const QString &)),
            this, SLOT(serviceRegistered()));
    connect(m_serviceWatcher, SIGNAL(serviceUnregistered(const QString &)),
            this, SLOT(serviceUnregistered()));

    QDBusReply<bool> registered= QDBusConnection::sessionBus().interface()->isServiceRegistered(SERVICE);
    if (registered.isValid())
    {
        if (registered.value())
        {
            serviceRegistered();
        };
    }
}

MusicIndicator::~MusicIndicator()
{
}

QString MusicIndicator::state() const
{
    if (innerIsValid())
    {
        return m_innerMI->state();
    }
    else
    {
        return "stopped";
    }
}

bool MusicIndicator::innerIsValid() const
{
    if (m_innerMI)
    {
        if (m_innerMI->isValid())
        {
            return true;
        }
    }
    return false;
}

void MusicIndicator::serviceRegistered()
{
    m_innerMI = new MusicServiceProxy(SERVICE, "/com/meego/app/music",
                                      QDBusConnection::sessionBus(), this);
    if (!m_innerMI->isValid())
    {
        QDBusError err = m_innerMI->lastError();
        qWarning() << "Failed to register for music events: " << err.message();
        serviceUnregistered();
    }
    else
    {
        connect(m_innerMI, SIGNAL(stateChanged()), this, SIGNAL(stateChanged()));
        emit stateChanged();
    }
}

void MusicIndicator::serviceUnregistered()
{
    if (m_innerMI)
    {
        m_innerMI->deleteLater();
    }
    m_innerMI = NULL;
    emit stateChanged();
}
