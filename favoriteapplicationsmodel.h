/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef FAVORITEAPPLICATIONSMODEL_H
#define FAVORITEAPPLICATIONSMODEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QFileSystemWatcher>
#include <QObject>
#include <QHash>
#include <QStringList>
#include <QSettings>
#include <QTimer>

#include "desktop.h"


class FavoriteApplicationsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int limit READ getLimit WRITE setLimit)
    Q_PROPERTY(QStringList filterOut READ getFilterOut WRITE setFilterOut)

public:
    explicit FavoriteApplicationsModel(QObject *parent = 0);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE void append(QString filename);

    void setLimit(int limit);
    int getLimit();

    void setFilterOut(QStringList filterOut);
    QStringList getFilterOut();

private slots:
    void readSettings();
    void writeSettings();
    void clearModel();
    void fullReset();
    //out of box experience init
    void oobeInit();

private:

    void delayedReset();
    QStringList m_desktopFiles;
    QList<Desktop *> m_favorites;
    QHash<QString,QVariant> m_popularityRating;
    QFileSystemWatcher *m_SettingFileWatcher;
    QSettings*  m_readSettings;
    int m_limit;
    QStringList m_filterOut;
};

#endif // RECENTAPPLICATIONSMODEL_H
