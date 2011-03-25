/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QHash>
#include <QDebug>
#include <QVariant>
#include <QProcess>
#include <QtDeclarative/qdeclarative.h>
#include <QTimerEvent>
#include "favoriteapplicationsmodel.h"
#include "desktop.h"

#define ORGANIZATION_NAME "MeeGo"
#define APPLICATION_NAME "favorite-applications"
#define DELAY 3000 //in ms

#define BROWSER "/usr/share/meego-ux-appgrid/applications/meego-app-browser.desktop"
#define PHOTOS  "/usr/share/meego-ux-appgrid/applications/meego-app-photos.desktop"
#define CHAT    "/usr/share/meego-ux-appgrid/applications/meego-app-im.desktop"
#define EMAIL   "/usr/share/meego-ux-appgrid/applications/meego-app-email.desktop"
#define MUSIC   "/usr/share/meego-ux-appgrid/applications/meego-app-music.desktop"
#define VIDEO   "/usr/share/meego-ux-appgrid/applications/meego-app-video.desktop"

#define LIMIT 6

FavoriteApplicationsModel::FavoriteApplicationsModel(QObject *parent) :
        QAbstractListModel(parent)
{

    QHash<int, QByteArray> roles;
    roles.insert(Desktop::Type, "type");
    roles.insert(Desktop::Title, "title");
    roles.insert(Desktop::Comment, "comment");
    roles.insert(Desktop::Icon, "icon");
    roles.insert(Desktop::Exec, "exec");
    roles.insert(Desktop::Filename, "filename");
    setRoleNames(roles);

    m_readSettings = new QSettings (ORGANIZATION_NAME, APPLICATION_NAME,this);
    m_readSettings->sync();

    readSettings();

    //Ensuring the file exists
    if ( !QFile(m_readSettings->fileName()).exists() )
    {
        QProcess::execute("touch " + m_readSettings->fileName() );
    }

    m_SettingFileWatcher= new QFileSystemWatcher(this);
    m_SettingFileWatcher->addPath(m_readSettings->fileName());
    connect(m_SettingFileWatcher,SIGNAL(fileChanged(QString)),this,SLOT(readSettings()));
}

void FavoriteApplicationsModel::oobeInit()
{

    m_desktopFiles << BROWSER << PHOTOS << CHAT << EMAIL << MUSIC << VIDEO;

    m_popularityRating.insert(BROWSER,1);
    m_popularityRating.insert(PHOTOS,1);
    m_popularityRating.insert(CHAT,1);
    m_popularityRating.insert(EMAIL,1);
    m_popularityRating.insert(MUSIC,1);
    m_popularityRating.insert(VIDEO,1);
}

int FavoriteApplicationsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return qMin( m_favorites.count(), LIMIT );
}

QVariant FavoriteApplicationsModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid() || index.row() > LIMIT)
        return QVariant();

    if (role == Desktop::Type)
        return m_favorites.at(index.row())->type();

    if (role == Desktop::Title)
        return m_favorites.at(index.row())->title();

    if (role == Desktop::Comment)
        return m_favorites.at(index.row())->comment();

    if (role == Desktop::Exec)
        return m_favorites.at(index.row())->exec();

    if (role == Desktop::Icon)
        return m_favorites.at(index.row())->icon();

    return m_favorites.at(index.row())->filename();
}

void FavoriteApplicationsModel::append(QString filename)
{

    int countdownIndex= m_desktopFiles.indexOf(filename);
    //Does the file already exist in the list
    if ( countdownIndex != -1 )
    {
        //it does? how popular is it
        int popularityRating
                =m_popularityRating.value( filename ).toInt();

        //Add one to it's popularity
        m_popularityRating.insert(filename,popularityRating + 1);

        //this application moves up in the countdown

        while((countdownIndex!=0) &&
              ((m_popularityRating.value( m_desktopFiles.at(countdownIndex - 1 ))).toInt()
               <= (m_popularityRating.value( m_desktopFiles.at(countdownIndex))).toInt()))
        {
            m_desktopFiles.swap(countdownIndex,countdownIndex - 1);
            m_favorites.swap(countdownIndex,countdownIndex - 1);

            countdownIndex= countdownIndex-1;
        }

        delayedReset();
    }
    //It does not let's add an entry for this application
    //in the popularity contest
    else
    {
        beginInsertRows( QModelIndex(), m_desktopFiles.count(), m_desktopFiles.count() );
        m_desktopFiles.append(filename);
        m_favorites.append(new Desktop(filename));
        m_popularityRating.insert(filename,0); 
        endInsertRows();
    }

     writeSettings();
}

void FavoriteApplicationsModel::writeSettings()
{
    disconnect(m_SettingFileWatcher,SIGNAL(fileChanged(QString)),this,SLOT(readSettings()));
    QSettings writeSettings(ORGANIZATION_NAME, APPLICATION_NAME);
    writeSettings.setValue("favorite-applications", m_desktopFiles);
    writeSettings.setValue("favorite-applications-ratings", m_popularityRating );
    writeSettings.sync();
    connect(m_SettingFileWatcher,SIGNAL(fileChanged(QString)),this,SLOT(readSettings()));
}

void FavoriteApplicationsModel ::readSettings()
{
    if (!m_readSettings)
        return;

    clearModel();

    m_desktopFiles = m_readSettings->value("favorite-applications").toStringList();
    if ( m_desktopFiles.count()==0 )
    {
        oobeInit();
    }
    else
        m_popularityRating = m_readSettings->value("favorite-applications-ratings").toHash();

    for (QStringList::const_iterator i = m_desktopFiles.begin(); i != m_desktopFiles.end(); i++)
    {
        m_favorites << new Desktop(*i);
    }

    delayedReset();
}

void FavoriteApplicationsModel::clearModel()
{
    m_desktopFiles.clear();
    m_popularityRating.clear();
    while (!m_favorites.isEmpty())
        delete m_favorites.takeFirst();
}

//The user of this model requires a delayed model reset
//for visual interface purposes
void FavoriteApplicationsModel::delayedReset()
{
     QTimer::singleShot(DELAY, this, SLOT(fullReset()));
}

void FavoriteApplicationsModel::fullReset()
{
    qDebug() << "DELAYED RESET";
    reset();
}

QML_DECLARE_TYPE(FavoriteApplicationsModel);
