/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "devicemodel.h"

#include <QDBusMessage>

#include <QDBusAbstractInterface>
#include <QDBusPendingCallWatcher>
#include <QString>

device::device(QDBusObjectPath DbusPath, org::freedesktop::UDisks* deviceInterFace, QObject *parent)
    :QObject(parent),
     m_isRemovable(undetermined),
     numofPropertiesLoaded(0),
     numofProperties(2),
     m_DbusPath(DbusPath),
     m_deviceInterface(deviceInterFace)
{
    determineifRemovable();
    determineLabel();
}

void device::determineifRemovable()
{

    QDBusMessage message = QDBusMessage::createMethodCall("org.freedesktop.UDisks",
                                                          m_DbusPath.path(),
                                                          QLatin1String("org.freedesktop.DBus.Properties"),
                                                          QLatin1String("Get"));

    QList<QVariant> arguments;
    arguments << "org.freedesktop.UDisks.Device" << "DeviceIsRemovable";
    message.setArguments(arguments);

    m_deviceInterface->connection().callWithCallback(message,this, SLOT(parseRemovableMessage(QDBusMessage)));

}

void device::determineLabel()
{

    QDBusMessage message = QDBusMessage::createMethodCall("org.freedesktop.UDisks",
                                                          m_DbusPath.path(),
                                                          QLatin1String("org.freedesktop.DBus.Properties"),
                                                          QLatin1String("Get"));

    QList<QVariant> arguments;
    arguments << "org.freedesktop.UDisks.Device" << "DriveModel";
    message.setArguments(arguments);

    m_deviceInterface->connection().callWithCallback(message,this, SLOT(parseLabelMessage(QDBusMessage)));


}


void device::parseRemovableMessage(const QDBusMessage &message)
{

    QDBusVariant dbusVariant= qvariant_cast<QDBusVariant>(message.arguments().at(0));
    if (dbusVariant.variant().toBool())
    {
        m_isRemovable=removable;
    }
    else
    {
        m_isRemovable=notRemovable;
    }

    maybeEmitLoaded();
}

void device::parseLabelMessage(const QDBusMessage &message)
{

    QDBusVariant dbusVariant= qvariant_cast<QDBusVariant>(message.arguments().at(0));
    m_label= dbusVariant.variant().toString();


    maybeEmitLoaded();
}

void device::maybeEmitLoaded()
{

    numofPropertiesLoaded++;

    if ( numofPropertiesLoaded == numofProperties )
    {

        emit loaded(this);
    }

}


devicemodel::devicemodel(QObject* parent)
    :QAbstractListModel(parent)
{

    QHash<int, QByteArray> roles;
    roles.insert( devicemodel::DeviceName, "deviceName");
    roles.insert( devicemodel::DeviceLabel, "deviceLabel");
    setRoleNames(roles);


    m_deviceInterface = new org::freedesktop::UDisks( QString("org.freedesktop.UDisks"),
                                                      QString("/org/freedesktop/UDisks"),
                                                      QDBusConnection::systemBus(), this);

    connect(m_deviceInterface,SIGNAL(DeviceAdded(QDBusObjectPath)),this,
            SLOT(addDevice(QDBusObjectPath)));


    connect(m_deviceInterface,SIGNAL(DeviceRemoved(QDBusObjectPath)),this,
            SLOT(removeDevice(QDBusObjectPath)));

    listDevices();
}



void devicemodel::listDevices()
{

    QDBusPendingReply<QList<QDBusObjectPath> > reply
            = m_deviceInterface->EnumerateDevices();

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);

    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(pushDeviceListtoModel(QDBusPendingCallWatcher*)));

}

void devicemodel::pushDeviceListtoModel(QDBusPendingCallWatcher* DBusWatcher)
{
    QDBusPendingReply< QList<QDBusObjectPath> > reply= *DBusWatcher;
    QList<QDBusObjectPath> deviceList = reply.value();


    QList<QDBusObjectPath>::const_iterator anIterator;

    for( anIterator = deviceList.constBegin();
    anIterator != deviceList.constEnd();
    anIterator++
            )
    {
        device* aDevice= new device(*anIterator, m_deviceInterface, this);
        connect( aDevice, SIGNAL(loaded(device*)), this, SLOT(addDevice(device*)));

    }

    delete DBusWatcher;

}


void devicemodel::addDevice(QDBusObjectPath objectPath)
{

    device* aDevice= new device(objectPath, m_deviceInterface, this);
    connect( aDevice,SIGNAL(loaded(device*)), this, SLOT(addDevice(device*)));
}

void devicemodel::addDevice(device *adevice)
{
    if ( adevice->getRemovable() == device::removable )
    {

        beginInsertRows( QModelIndex(), m_devicesList.count(), m_devicesList.count() );
        m_devicesList.append(adevice);
        emit modelChanged();
        endInsertRows();
    }
    else
    {
        delete adevice;
    }

}


void devicemodel::removeDevice(QDBusObjectPath objectPath)
{


    QList<device*>::iterator anIterator;
    for (
    anIterator = m_devicesList.begin();
    anIterator != m_devicesList.end();
    anIterator++
            )
    {

        if ( (*anIterator)->path() == objectPath.path() )
        {

            int index =anIterator - m_devicesList.begin();
            device* devicetoRemove= (*anIterator);

            beginRemoveRows(QModelIndex(), index ,index );
            m_devicesList.erase(anIterator);
            emit modelChanged();
            endRemoveRows();

            delete devicetoRemove;
            break;
        }

    }
}


int devicemodel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_devicesList.count();
}

QVariant devicemodel::data(const QModelIndex &index, int role) const
{

    if ( index.row() < m_devicesList.count() )
    {
        if (role == devicemodel::DeviceLabel )
            return (m_devicesList.at(index.row()))->label();
        else if ( role == devicemodel::DeviceName )
            return (m_devicesList.at(index.row()))->path();
    }

    return QVariant();

}

QML_DECLARE_TYPE(devicemodel);

