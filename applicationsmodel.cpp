/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QtDeclarative/qdeclarative.h>
#include <QDir>
#include <QFileInfoList>
#include <QFileSystemWatcher>
#include <QRegExp>
#include <mdesktopentry.h>

#include "applicationsmodel.h"
#include "desktop.h"
#include "favoriteapplicationsmodel.h"
#include "recentapplicationsmodel.h"
#include "appupappsmodel.h"

ApplicationsModel::ApplicationsModel(QObject *parent) :
    QAbstractListModel(parent),
    m_type("Application"),
    m_recents(new RecentApplicationsModel(this)),
    m_favorites(new FavoriteApplicationsModel(this)),
    m_featuredApps(0),
    m_updatedApps(0)

{
    m_watcher = new QFileSystemWatcher(this);
    connect(m_watcher, SIGNAL(directoryChanged(QString)), this, SLOT(appsDirChanged(QString)));

    // Default dirs
    m_watcher->addPath("/usr/share/applications");
    m_watcher->addPath("/usr/share/meego-ux-appgrid/applications");

    QHash<int, QByteArray> roles;
    roles[id]="id";
    roles[name]="name";
    roles[exec]="exec";
    roles[custom]="custom";

    setRoleNames(roles);

}

void ApplicationsModel::appsDirChanged(QString changedDir)
{
  resetApps();
  emit appsChanged();
}

void ApplicationsModel::setDirectories(QStringList directories)
{
    m_watcher->removePaths(m_watcher->directories());

    foreach(QString directory, directories)
    {
        QString path;
        if (directory.startsWith("~"))
            path = QDir::homePath() + directory.remove(0, 1);
        else if (directory == ".")
            path = QDir::currentPath();
        else
            path = directory;

        // If directory doesn't exist, then attempt to create it
        if (!QDir(path).exists())
        {
            QDir().mkpath(path);
        }

        m_watcher->addPath(path);
    }

    resetApps();
}

void ApplicationsModel::resetApps()
{
    while (!m_apps.isEmpty())
        delete m_apps.takeFirst();

    QStringList addedDirectories;

    foreach (QString target, m_watcher->directories())
    {
        QDir dir(target);
        dir.setFilter(QDir::Files | QDir::NoSymLinks);

        QStringList filters;
        filters << "*.desktop";
        foreach (QFileInfo fileInfo, dir.entryInfoList(filters))
        {
            if(addedDirectories.contains(fileInfo.fileName()))
                continue;

            addedDirectories << fileInfo.fileName();

            Desktop *desktopEntry = new Desktop(fileInfo.absoluteFilePath());
            if (!desktopEntry->isValid() || (desktopEntry->type() != m_type))
            {
                delete desktopEntry;
                continue;
            }

            m_apps << desktopEntry;
        }
    }


    emit appsReset();
}

QVariant ApplicationsModel::data(const QModelIndex& index, int role) const
{
    if(role == name)
    {
        if(index.row() < m_apps.count())
        {
            return m_apps[index.row()]->title();
        }
    }
    else if(role == exec)
    {
        if(index.row() < m_apps.count())
        {
            return m_apps[index.row()]->exec();
        }
    }
    else if(role == custom)
    {
        if(index.row() < m_apps.count())
        {
            return m_apps[index.row()]->value(m_customValue);
        }
    }
    return QVariant();
}

QDeclarativeListProperty<Desktop> ApplicationsModel::apps()
{
    return QDeclarativeListProperty<Desktop>(this, m_apps);
}

RecentApplicationsModel *ApplicationsModel::recents()
{
    return m_recents;
}

FavoriteApplicationsModel *ApplicationsModel::favorites()
{
    return m_favorites;
}

AppUpAppsModel *ApplicationsModel::appupFeatured()
{
    if (!m_featuredApps)
        m_featuredApps = new AppUpAppsModel(AppUpAppsModel::Featured, this);
    return m_featuredApps;
}

AppUpAppsModel *ApplicationsModel::appupUpdated()
{
    if (!m_updatedApps)
        m_updatedApps = new AppUpAppsModel(AppUpAppsModel::Updated, this);
    return m_updatedApps;
}

QString ApplicationsModel::value(QString id, QString key)
{
    foreach(Desktop* item, m_apps)
    {
        if(item->id() == id)
            return item->value(key);
    }
    return "";
}

void ApplicationsModel::launchDesktop(QObject *object)
{

    Desktop *target = qobject_cast<Desktop *>(object);
    if (!target)
        return;

    m_favorites->append(target->filename());
    m_recents->append(target->filename());

    QString cmd = target->exec();

    // http://standards.freedesktop.org/desktop-entry-spec/latest/ar01s06.html
    cmd.replace(QRegExp("%k"), target->filename());
    cmd.replace(QRegExp("%i"), QString("--icon ") + target->icon());
    cmd.replace(QRegExp("%c"), target->title());
    cmd.replace(QRegExp("%[fFuU]"), target->filename()); // stuff we don't handle

    QProcess::startDetached(cmd);

    emit startingApp(target->title(), target->icon());
}

void ApplicationsModel::launchDesktopByName(QString name)
{

    if (m_apps.length() == 0)
        return;

    for (int i = 0; i < m_apps.length(); i++)
    {
        Desktop *d = m_apps[i];
        if (d->filename().contains(name))
        {
            launchDesktop(d);
            return;
        }
    }
}

void ApplicationsModel::launch(QString cmd)
{
    QProcess::startDetached(cmd);
}

ApplicationsModel::~ApplicationsModel()
{
    while (!m_apps.isEmpty())
        delete m_apps.takeFirst();
}

QML_DECLARE_TYPE(ApplicationsModel);
