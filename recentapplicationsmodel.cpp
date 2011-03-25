/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QHash>
#include <QtDeclarative/qdeclarative.h>
#include <mgconfitem.h>
#include "recentapplicationsmodel.h"
#include "desktop.h"

#define ORGANIZATION_NAME "MeeGo"
#define APPLICATION_NAME "meego-ux-components"
#define GCONF_KEY "/meego/ux/recent-applications"

RecentApplicationsModel::RecentApplicationsModel(QObject *parent) :
    QAbstractListModel(parent),
    m_limit(0)
{
    QHash<int, QByteArray> roles;
    roles.insert(Desktop::Type, "type");
    roles.insert(Desktop::Title, "title");
    roles.insert(Desktop::Comment, "comment");
    roles.insert(Desktop::Icon, "icon");
    roles.insert(Desktop::Exec, "exec");
    roles.insert(Desktop::Filename, "filename");
    setRoleNames(roles);

    m_gitem = new MGConfItem(GCONF_KEY, this);
    connect(m_gitem, SIGNAL(valueChanged()), this, SLOT(recentAppsChanged()));

    QStringList desktops = m_gitem->value().toStringList();
    for (QStringList::const_iterator i = desktops.begin(); i != desktops.end(); i++)
    {
        m_recents << new Desktop(*i);
    }
}

RecentApplicationsModel::~RecentApplicationsModel()
{
    while (!m_recents.isEmpty())
        delete m_recents.takeLast();
}

int RecentApplicationsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (m_limit == 0)
        return m_recents.count();
    else
        return std::min(m_recents.count(), m_limit);
}

QVariant RecentApplicationsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() > m_recents.count())
        return QVariant();

    if (role == Desktop::Type)
        return m_recents[index.row()]->type();

    if (role == Desktop::Title)
        return m_recents[index.row()]->title();

    if (role == Desktop::Comment)
        return m_recents[index.row()]->comment();

    if (role == Desktop::Exec)
        return m_recents[index.row()]->exec();

    if (role == Desktop::Icon)
        return m_recents[index.row()]->icon();

    return m_recents[index.row()]->filename();
}

void RecentApplicationsModel::append(QString filename)
{

    QList<Desktop *> tmp;
    while (!m_recents.isEmpty())
    {
        Desktop *d = m_recents.takeLast();
        if (d->filename() == filename)
        {
            delete d;
        }
        else
        {
            tmp << d;
        }
    }
    m_recents = tmp;
    m_recents.prepend(new Desktop(filename));

    saveRecents();
}

void RecentApplicationsModel::remove(QString filename)
{
    // Remove all occurances of a given desktop entry
    QList<Desktop *> tmp;
    while (!m_recents.isEmpty())
    {
        Desktop *d = m_recents.takeLast();
        if (d->filename() == filename)
            d->deleteLater();
        else
            tmp << d;
    }
    m_recents = tmp;
    saveRecents();
}

void RecentApplicationsModel::saveRecents()
{
    QStringList desktops;
    foreach (Desktop *d, m_recents) {
        desktops << d->filename();
    }

    m_gitem->set(desktops);
    emit modelReset();
}

void RecentApplicationsModel::recentAppsChanged()
{
    while (!m_recents.isEmpty())
        delete m_recents.takeLast();

    QStringList desktops = m_gitem->value().toStringList();
    for (QStringList::const_iterator i = desktops.begin(); i != desktops.end(); i++)
    {
        m_recents << new Desktop(*i);
    }
}

QML_DECLARE_TYPE(RecentApplicationsModel);
