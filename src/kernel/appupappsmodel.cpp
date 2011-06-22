/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "appupappsmodel.h"

#include <QHash>
#include <QDir>
#include <QDebug>
#include <QStringList>
#include <QFileSystemWatcher>
#include <QtDeclarative/qdeclarative.h>
#include <mdesktopentry.h>


AppUpAppsModel::AppUpAppsModel(AppUpType type, QObject *parent) :
    QAbstractListModel(parent),
    mLimit(6),
    mType(type)
{
    QHash<int, QByteArray> roles;
    roles.insert(AppUpAppsModel::Type, "type");
    roles.insert(AppUpAppsModel::Title, "title");
    roles.insert(AppUpAppsModel::Comment, "comment");
    roles.insert(AppUpAppsModel::Icon, "icon");
    roles.insert(AppUpAppsModel::Exec, "exec");
    roles.insert(AppUpAppsModel::Filename, "filename");
    setRoleNames(roles);

    mWatcher = new QFileSystemWatcher(this);
    qDebug() << "Type: " << type << "Adding path: " << APPUP_DESKTOP_PATH.arg(APPUP_DIRECTORIES.at(mType));
    mWatcher->addPath(APPUP_DESKTOP_PATH.arg(QDir::homePath(), APPUP_DIRECTORIES.at(mType)));

    connect(mWatcher,
            SIGNAL(directoryChanged(QString)),
            this,
            SLOT(loadDesktops()));
    loadDesktops();
}

AppUpAppsModel::~AppUpAppsModel()
{
}

int AppUpAppsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (mLimit == 0)
        return mDesktops.count();
    else
        return std::min(mDesktops.count(), mLimit);
}

QVariant AppUpAppsModel::data(const QModelIndex &index, int role) const
{
    int row;
    if (!index.isValid() || index.row() >= mDesktops.count())
        return QVariant();
    row = index.row();

    switch (role) {
    case AppUpAppsModel::Type:
        return mDesktops[row]->type();
        break;
    case AppUpAppsModel::Title:
        return mDesktops[row]->name();
        break;
    case AppUpAppsModel::Comment:
        return mDesktops[row]->comment();
        break;
    case AppUpAppsModel::Exec:
        return mDesktops[row]->exec();
        break;
    case AppUpAppsModel::Icon:
        return mDesktops[row]->icon();
        break;
    case AppUpAppsModel::Filename:
        return mDesktops[row]->fileName();
        break;
    default:
        qDebug("Unhandled data role requested in AppUpAppsModel::data(): %i", role);
        return QVariant();
        break;
    }

}

//Private slots:

void AppUpAppsModel::loadDesktops()
{
//    qDebug("**** Beginning loadDesktops");
    if (mDesktops.count()) {
        this->beginRemoveRows(QModelIndex(), 0, mDesktops.count()-1);
        mDesktops.clear();
        this->endRemoveRows();
    }

    qDebug() << mWatcher->directories();

    if (mWatcher->directories().count() < 1) {
        qDebug("No directories loaded into mWatcher!");
        return;
    }
    QString curDir = mWatcher->directories().at(0);
    QDir dir(curDir);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);

    QStringList filters;
    filters << "*.desktop";
    foreach (QFileInfo fileInfo, dir.entryInfoList(filters))
    {
//        qDebug("Inside foreach loop");
        MDesktopEntry *newDesktop = new MDesktopEntry(fileInfo.absoluteFilePath());
//        qDebug("After MDesktopEntry creation");
        if (newDesktop->isValid()) {
//            qDebug("MDesktopEntry valid!");
            this->beginInsertRows(QModelIndex(), mDesktops.count(), mDesktops.count());
            mDesktops.append(new MDesktopEntry(fileInfo.absoluteFilePath()));
            this->endInsertRows();
//            qDebug("After endInsertRows");
        }
    }
//    qDebug("End of loadDesktops");
}

//Private functions

void AppUpAppsModel::setType(AppUpType type)
{
    mType = type;
    emit this->typeChanged();
    mWatcher->removePath(mWatcher->directories()[0]);
    mWatcher->addPath(APPUP_DESKTOP_PATH.arg(QDir::homePath(), APPUP_DIRECTORIES.at(mType)));
    loadDesktops();

}


QML_DECLARE_TYPE(AppUpAppsModel);
