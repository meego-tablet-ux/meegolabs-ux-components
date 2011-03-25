/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef APPLICATIONSMODEL_H
#define APPLICATIONSMODEL_H

#include <QObject>
#include <QProcess>
#include <QtDeclarative>
#include <QAbstractListModel>

#include <mdesktopentry.h>

#include "desktop.h"

class RecentApplicationsModel;
class FavoriteApplicationsModel;
class QFileSystemWatcher;

class ApplicationsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QStringList directories READ directories WRITE setDirectories)
    Q_PROPERTY(QString directory READ directory WRITE setDirectory)
    Q_PROPERTY(QDeclarativeListProperty<Desktop> apps READ apps NOTIFY appsChanged)
    Q_PROPERTY(RecentApplicationsModel* recents READ recents NOTIFY recentsChanged)
    Q_PROPERTY(FavoriteApplicationsModel* favorites READ favorites)
    Q_PROPERTY(QString customField READ customValue WRITE setCustomValue)
    Q_PROPERTY(QString type READ type WRITE setType)

public:
    explicit ApplicationsModel(QObject *parent = 0);
    ~ApplicationsModel();

    enum Role
    {
        id = Qt::UserRole + 1,
        name,
        exec,
        custom
    };

    QDeclarativeListProperty<Desktop> apps();
    RecentApplicationsModel *recents();
    FavoriteApplicationsModel *favorites();

    QString directory() const {
        qDebug("Warning, 'directory' has been deprecated. Use 'directories' instead.");
        return m_watcher->directories().at(0);
    }

    QStringList directories() const {
        return m_watcher->directories();
    }

    void setDirectory(QString dir){ qDebug("Warning, directory property is deprecated. Use 'directories' instead"); setDirectories(QStringList()<<dir); }
    void setDirectories(QStringList directory);

    void setCustomValue(QString value) { m_customValue = value; }
    QString customValue() { return m_customValue; }

    void setType(QString value) { m_type = value; resetApps();}
    QString type() { return m_type; }

    ///overrides from QAbstractModel:
    int rowCount(const QModelIndex & = QModelIndex()) const { return m_apps.count(); }
    int columnCount(const QModelIndex & = QModelIndex()) const { return 1; }
    QVariant data(const QModelIndex& index, int role) const;

signals:
    void appsReset();
    void appsChanged();
    void recentsChanged();
    void startingApp(QString title, QString icon);

public slots:
    QString value(QString id, QString key);
    void launchDesktop(QObject *);
    void launchDesktopByName(QString name);
    void launch(QString);
    void appendThemeSearchPath(QString path);

private slots:
    void appsDirChanged(QString);
    void resetApps();

private:
    QList<Desktop *> m_apps;
    QString m_customValue;
    QString m_type;
    RecentApplicationsModel *m_recents;
    FavoriteApplicationsModel* m_favorites;
    QFileSystemWatcher *m_watcher;

    Q_DISABLE_COPY(ApplicationsModel)
};

#endif // APPLICATIONSMODEL_H
