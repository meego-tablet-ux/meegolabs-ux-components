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
#include <QTimer>
#include <QtDeclarative/qdeclarative.h>
#include <mdesktopentry.h>

AppUpAppsModel::AppUpAppsModel(AppUpType type, QObject *parent) :
    QAbstractListModel(parent),
    mReloadTimer(new QTimer(this)),
    mLastReload(QDateTime::currentDateTime().addSecs(-100)),
    mLimit(6),
    mType(type),
    mRecurseCount(0),
    mNeedReload(false)
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
    mReloadTimer->setSingleShot(true);
    connect(mReloadTimer,
            SIGNAL(timeout()),
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
    //Only allow a reload at a maximum of every 5 seconds, to allow the desktop file writing
    //to quiesce before we reload
    //Also, protect against significant system time changes preventing us from refreshing
    //for 10 years...
    if ((mLastReload >= QDateTime::currentDateTime().addSecs(-5)) &&
            !(mLastReload > QDateTime::currentDateTime()) ) {
        mReloadTimer->start(5100);
        return;
    }
    //Keep us from reentering loadDesktops while we're still processing the desktops from last time
    if (mMutex.tryLock(100)) {
        mNeedReload = false;
        mLastReload = QDateTime::currentDateTime();

        if (mDesktops.count()) {
            this->beginRemoveRows(QModelIndex(), 0, mDesktops.count()-1);
            mDesktops.clear();
            this->endRemoveRows();
        }

        if (mWatcher->directories().count() < 1) {
            qDebug("No directories loaded into mWatcher!");
            mMutex.unlock();
            return;
        }
        QString curDir = mWatcher->directories().at(0);
        QDir dir(curDir);
        dir.setFilter(QDir::Files | QDir::NoSymLinks);

        QStringList filters;
        filters << "*.desktop";
        foreach (QFileInfo fileInfo, dir.entryInfoList(filters)) {
            qDebug() << fileInfo.absoluteFilePath();
            MDesktopEntry *newDesktop = new MDesktopEntry(fileInfo.absoluteFilePath());
            if (newDesktop->isValid()) {
                this->beginInsertRows(QModelIndex(), mDesktops.count(), mDesktops.count());
                mDesktops.append(new MDesktopEntry(fileInfo.absoluteFilePath()));
                this->endInsertRows();
            } else {
                qDebug() << "Invalid desktop: " << fileInfo.absoluteFilePath();
            }
        }
        emit countChanged();
        mMutex.unlock();

        //If new changes came in while we were still processing, recurse
        //and run again...        
        if (mRecurseCount >= 5) {
            //However, if we've recursed 5 times, die out and ignore the
            //changes until the next time we get a change event - it's
            //better to rarely miss a change for a while than to
            //introduce a potential infinite recursion loop in a core
            //component...
            mRecurseCount = 0;
            mNeedReload = false;
            qDebug("AppUpAppsModel: Had to bail out of loadDesktops due to recursion! Current count: %u", mDesktops.count());
        } else if (mNeedReload) {
            mRecurseCount++;
            loadDesktops();
            mRecurseCount = 0;
        }
    } else {
        //If we're still processing, and get a new change, then indicate that
        //we need reload the desktops once we're done this time - this method
        //will make our worst case a 2x processing of desktops - 1st time will
        //hit, start processing, all concurrent changes will just flag
        //mNeedReload, then once we're done with the first pass, we will
        //recurse back, run the final processing.
        mNeedReload = true;
    }
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
