/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QDBusConnection>
#include <QtDeclarative>
#include <QObject>
#include "udisk_interface.h"

class device
    :public QObject
{
    Q_OBJECT
public:
    enum DEVICEREMOVABLETYPE { undetermined, removable, notRemovable };

    explicit device( QDBusObjectPath m_DbusPath, org::freedesktop::UDisks* deviceInterFace, QObject* parent=0 );

    void setDbusInterface( org::freedesktop::UDisks* deviceInterFace)
    {
        m_deviceInterface = deviceInterFace;
    }

    QString path()  { return m_DbusPath.path(); }
    QString label() { return m_label; }

    DEVICEREMOVABLETYPE getRemovable() { return m_isRemovable; }


signals:
    void loaded(device*);

private:
    void maybeEmitLoaded();

    void determineifRemovable();
    void determineLabel();

    DEVICEREMOVABLETYPE m_isRemovable;
    QString m_label;


    uint  numofPropertiesLoaded;
    uint  numofProperties;

    QDBusObjectPath m_DbusPath;
    org::freedesktop::UDisks* m_deviceInterface;

private slots:
    void parseRemovableMessage(const QDBusMessage &message);
    void parseLabelMessage    (const QDBusMessage &message);

};

class devicemodel
    :public QAbstractListModel
{
    enum Role {
        DeviceName = Qt::UserRole + 1,
        DeviceLabel = Qt::UserRole + 2
   };

    Q_PROPERTY(int count READ getCount NOTIFY modelChanged);

    typedef QList<QDBusObjectPath> DBusReplyList;

    Q_OBJECT

public:
    explicit devicemodel(QObject* parent=0);
    int getCount() { return rowCount(); }

signals:
    void modelChanged();

public slots:
    void addDevice(QDBusObjectPath objectPath);
    void removeDevice(QDBusObjectPath objectPath);

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

private slots:
    void addDevice(device*);
    void pushDeviceListtoModel(QDBusPendingCallWatcher*);
    void listDevices();

private:
    org::freedesktop::UDisks* m_deviceInterface;


    QList<device*> m_devicesList;

};

#endif // DEVICEMODEL_H
