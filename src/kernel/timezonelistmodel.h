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
#include "meegolocale.h"

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
        GMTOffset,
        Latitude,
        Longitude,
        CountryCode,
        Index,
        Region,
        LocationName,
        LongGMTName,
        GMTName
    };

    Q_ENUMS(Role);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE QVariant getData(int index, int role) const
    {
        return data(this->index(index), role);
    }

    QStringList regions();

public slots:
    void filterOut(QString filter);
    QString getLocationName(QString title) const;
    QString getLongGMTName(QString title) const;
    QString getGMTName(QString title) const;

protected:
    class TimezoneItem {
    public:
        TimezoneItem(KTimeZone zone,
		     QString location,
		     QString longgmt,
		     QString gmt,
		     meego::Locale * pLocale) :
            timezone(zone),
            locationName(location),
            longGMTName(longgmt),
            GMTName(gmt),
	    mpLocale(pLocale)
       {}

        bool operator< (const TimezoneItem & other) const;

        KTimeZone timezone;
        QString locationName;
        QString longGMTName;
        QString GMTName;
	meego::Locale * mpLocale;
    };

    KSystemTimeZones zones;
    QList<TimezoneItem> itemsList;
    QList<TimezoneItem*> itemsDisplay;
    meego::Locale mLocale;

    static bool LessTimezone(const TimezoneItem & leftZone, 
			     const TimezoneItem & rightZone);

};

#endif // TIMEZONELISTMODEL_H
