/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QDir>
#include <QDebug>
#include "timezonelistmodel.h"

TimezoneListModel::TimezoneListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles.insert(Title, "title");
    roles.insert(City, "city");
    roles.insert(GMTOffset, "gmtoffset");
    roles.insert(Latitude, "latitude");
    roles.insert(Longitude, "longitude");
    roles.insert(CountryCode, "countrycode");
    roles.insert(Index, "index");
    roles.insert(Region,"region");
    setRoleNames(roles);

    KTimeZones *temp = zones.timeZones();
    QMap<QString, KTimeZone> map = temp->zones();
    itemsList = map.values();
    for(int i = 0; i < itemsList.count(); i++)
    {

        itemsDisplay << &itemsList[i];
        qDebug()<<"Title: "<<itemsDisplay[i]->name();
        qDebug()<<"City: "<<getCity(itemsDisplay[i]->name());
        qDebug()<<"Country Code: "<<itemsDisplay[i]->countryCode();
        qDebug()<<"Region: "<<itemsDisplay[i]->name().split("/").at(0);
    }

    qDebug()<<"Number of items: "<<itemsDisplay.count();
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

QString TimezoneListModel::getCity(QString title) const
{
    QStringList temp = title.split("/", QString::SkipEmptyParts);
    QString res = temp.last();
    res.replace("_", " ");
    return res;
}

QStringList TimezoneListModel::regions()
{
    QStringList list;
    QMap<QString,QString> map;

    foreach(KTimeZone* item, itemsDisplay)
    {
        map[item->name().split("/").at(0)] = "nothing";
    }

    foreach(QString key, map.keys())
    {
        list<<key;
    }

    return list;
}

void TimezoneListModel::filterOut(QString filter)
{
    QList<KTimeZone*> displaylist;
    for(int i = 0; i < itemsList.count(); i++)
        if(filter.isEmpty()||itemsList[i].name().contains(filter, Qt::CaseInsensitive))
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
        return itemsDisplay[index.row()]->name();

    if (role == City)
        return getCity(itemsDisplay[index.row()]->name());

    if (role == GMTOffset)
        return itemsDisplay[index.row()]->currentOffset(Qt::UTC)/3600;

    if (role == Latitude)
        return itemsDisplay[index.row()]->latitude();

    if (role == Longitude)
        return itemsDisplay[index.row()]->longitude();

    if (role == CountryCode)
        return itemsDisplay[index.row()]->countryCode();

    if (role == Index)
        return index.row();

    if (role == Region)
        return itemsDisplay[index.row()]->name().split("/").at(0);

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
