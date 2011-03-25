/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef TIMEZONELISTMODEL_H
#define TIMEZONELISTMODEL_H

#include <QAbstractListModel>
#include <QtCore/QtCore>
#include <QtCore/QObject>
#include <kcalcoren/ksystemtimezone.h>

class TimezoneListModel: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount)
    Q_PROPERTY(QStringList regions READ regions)

public:
    TimezoneListModel(QObject *parent = 0);
    ~TimezoneListModel();

    enum Role {
        Title = Qt::UserRole + 1,
        City = Qt::UserRole + 2,
        GMTOffset = Qt::UserRole + 3,
        Latitude = Qt::UserRole + 4,
        Longitude = Qt::UserRole + 5,
        CountryCode = Qt::UserRole + 6,
        Index = Qt::UserRole + 7,
        Region
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QStringList regions();

public slots:
    void filterOut(QString filter);

protected:
    QString getCity(QString title) const;
    KSystemTimeZones zones;
    QList<KTimeZone> itemsList;
    QList<KTimeZone*> itemsDisplay;
};

#endif // TIMEZONELISTMODEL_H
