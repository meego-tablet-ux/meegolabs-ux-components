/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QDir>
#include <QDebug>
#include <unicode/timezone.h>
#include <unicode/locid.h>
#include "timezonelistmodel.h"

TimezoneListModel::TimezoneListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles.insert(Title, "title");
    roles.insert(GMTOffset, "gmtoffset");
    roles.insert(Latitude, "latitude");
    roles.insert(Longitude, "longitude");
    roles.insert(CountryCode, "countrycode");
    roles.insert(Index, "index");
    roles.insert(Region,"region");
    roles.insert(LocationName, "locationname");
    roles.insert(LongGMTName, "longgmtname");
    roles.insert(GMTName, "gmtname");
    setRoleNames(roles);

    foreach (KTimeZone zone, zones.timeZones()->zones()) {
        itemsList.append(TimezoneItem(zone,
                    getLocationName(zone.name()),
                    getLongGMTName(zone.name()),
                    getGMTName(zone.name())));
    }

    for(int i = 0; i < itemsList.count(); i++)
    {
        itemsDisplay << &itemsList[i];
    }

}

TimezoneListModel::~TimezoneListModel()
{
    if(!itemsList.isEmpty())
    {
        /* formally remove all the items from the list */
        beginRemoveRows(QModelIndex(), 0, itemsList.count()-1);
        itemsList.clear();
        endRemoveRows();
    }
}

QString TimezoneListModel::getLocationName(QString title) const
{
    TimeZone *zone = TimeZone::createTimeZone(UnicodeString(static_cast<const UChar*>(title.utf16())));
    UnicodeString result;
    zone->getDisplayName(TRUE, TimeZone::GENERIC_LOCATION, Locale(mLocale.locale().toAscii().constData()), result);
    delete zone;
    return QString(reinterpret_cast<const QChar*>(result.getBuffer()), result.length());
}

QString TimezoneListModel::getLongGMTName(QString title) const
{
    TimeZone *zone = TimeZone::createTimeZone(UnicodeString(static_cast<const UChar*>(title.utf16())));
    UnicodeString result;
    zone->getDisplayName(TRUE, TimeZone::LONG_GMT, Locale(mLocale.locale().toAscii().constData()), result);
    delete zone;
    return QString(reinterpret_cast<const QChar*>(result.getBuffer()), result.length());
}

QString TimezoneListModel::getGMTName(QString title) const
{
    TimeZone *zone = TimeZone::createTimeZone(UnicodeString(static_cast<const UChar*>(title.utf16())));
    UnicodeString result;
    zone->getDisplayName(TRUE, TimeZone::SHORT_GMT, Locale(mLocale.locale().toAscii().constData()), result);
    delete zone;
    return QString(reinterpret_cast<const QChar*>(result.getBuffer()), result.length());
}

QStringList TimezoneListModel::regions()
{
    QStringList list;
    QMap<QString,QString> map;

    foreach(TimezoneItem* item, itemsDisplay)
    {
        map[item->timezone.name().split("/").at(0)] = "nothing";
    }

    foreach(QString key, map.keys())
    {
        list<<key;
    }

    return list;
}

void TimezoneListModel::filterOut(QString filter)
{
    QList<TimezoneItem*> displaylist;
    for(int i = 0; i < itemsList.count(); i++)
        if(filter.isEmpty() ||
            itemsList[i].locationName.contains(filter, Qt::CaseInsensitive))
            displaylist << &itemsList[i];

    if(!itemsDisplay.isEmpty())
    {
        beginRemoveRows(QModelIndex(), 0, itemsDisplay.count()-1);
        itemsDisplay.clear();
        endRemoveRows();
    }
    if(!displaylist.isEmpty())
    {
        beginInsertRows(QModelIndex(), 0, displaylist.count()-1);
        itemsDisplay = displaylist;
        endInsertRows();
    }
}

QVariant TimezoneListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() > itemsDisplay.count())
        return QVariant();

    if (role == Title)
        return itemsDisplay[index.row()]->timezone.name();

    if (role == GMTOffset)
        return itemsDisplay[index.row()]->timezone.currentOffset(Qt::UTC);

    if (role == Latitude)
        return itemsDisplay[index.row()]->timezone.latitude();

    if (role == Longitude)
        return itemsDisplay[index.row()]->timezone.longitude();

    if (role == CountryCode)
        return itemsDisplay[index.row()]->timezone.countryCode();

    if (role == Index)
        return index.row();

    if (role == Region)
        return itemsDisplay[index.row()]->timezone.name().split("/").at(0);

    if (role == LocationName)
        return itemsDisplay[index.row()]->locationName;

    if (role == LongGMTName)
        return itemsDisplay[index.row()]->longGMTName;

    if (role == GMTName)
        return itemsDisplay[index.row()]->GMTName;

    return QVariant();
}

int TimezoneListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return itemsDisplay.size();
}

int TimezoneListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1;
}
