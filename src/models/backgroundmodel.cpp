/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QString>
#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>
#include <mgconfitem.h>

#include "backgroundmodel.h"

#define BACKGROUND_KEY "/meego/ux/background"
#define THEME_KEY "/meego/ux/theme"

BackgroundModel::BackgroundModel(QObject* parent) :
        QAbstractListModel(parent)
{
    //m_path = QString("/usr/share/wallpapers/%1/backgrounds/").arg()
    QHash<int, QByteArray> roles;
    roles.insert(BackgroundModel::Uri, "uri");
    roles.insert(BackgroundModel::Active, "active");
    setRoleNames(roles);

    themeGConfItem = new MGConfItem(THEME_KEY, this);
    m_path = "/usr/share/themes/" + themeGConfItem->value().toString() + "/backgrounds/";
    if (!QDir().exists(m_path))
    {
        // fallback to good ole 1024x600
        m_path = "/usr/share/themes/1024-600-10/backgrounds/";
    }
    connect(themeGConfItem, SIGNAL(valueChanged()), this, SLOT(themeChanged()));

    watcher = new QFileSystemWatcher;
    watcher->addPath(m_path);

    backgroundGConfItem = new MGConfItem(BACKGROUND_KEY, this);
    connect(backgroundGConfItem, SIGNAL(valueChanged()), this, SLOT(backgroundChanged()));

    selected = backgroundGConfItem->value().toString();
    if (!QFile::exists(selected))
    {
        QDir dir(m_path);
        selected = dir.entryList(QDir::Files).first();
        backgroundGConfItem->set(m_path + selected);
    }
    filesystemUpdate();
}

void BackgroundModel::themeChanged()
{
    watcher->removePath(m_path);
    m_path = "/usr/share/themes/" + themeGConfItem->value().toString() + "/backgrounds/";
    watcher->addPath(m_path);
    filesystemUpdate();
}

int BackgroundModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return wallpapers.count();
}

QVariant BackgroundModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index);
    Q_UNUSED(role);

    if (index.row() > wallpapers.count())
        return QVariant();

    if (role == BackgroundModel::Uri)
    {
        return m_path + wallpapers[index.row()];
    }
    else
    {
        return selected == m_path + wallpapers[index.row()];
    }
}

void BackgroundModel::filesystemUpdate(QString file)
{
    Q_UNUSED(file);
    QDir dir(m_path);

    bool foundActiveWallpaper = false;

    wallpapers.clear();
    foreach (QString entry, dir.entryList(QDir::Files))
    {
        if (m_path + entry != selected)
            wallpapers << entry;
        else
            foundActiveWallpaper = true;
    }
    if (foundActiveWallpaper)
    {
        QString tmp = selected;
        wallpapers.prepend(tmp.replace(m_path, QString()));
    }
    emit modelReset();
}

void BackgroundModel::select(int index)
{
    if (index > -1 && index < wallpapers.count())
    {
        selected = wallpapers[index];
        backgroundGConfItem->set(m_path + selected);
        emit modelReset();
    }
}

void BackgroundModel::backgroundChanged()
{
    selected = backgroundGConfItem->value().toString();
    filesystemUpdate();
    emit activeWallpaperChanged();
}

QString BackgroundModel::copyImageToBackgrounds(QString path)
{
    // normalize the destination path string
    QString destPath = m_path;
    if (destPath.right(1) != "/")
        destPath.append("/");
    destPath.append(path.mid(path.lastIndexOf('/') + 1));
    QFile::copy(path, destPath);

    // return the path we copied to
    return destPath;
}

void BackgroundModel::setActiveWallpaper(QString file)
{
    if (file.startsWith("file://"))
        backgroundGConfItem->set(file.mid(7));
    else
        backgroundGConfItem->set(file);
    emit activeWallpaperChanged();
}

QML_DECLARE_TYPE(BackgroundModel);
