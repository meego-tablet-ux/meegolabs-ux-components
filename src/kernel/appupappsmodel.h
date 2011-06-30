/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef APPUPAPPSMODEL_H
#define APPUPAPPSMODEL_H

#include <QObject>
#include <QString>
#include <QList>
#include <QAbstractListModel>
#include <QMutex>

#define APPUP_DESKTOP_PATH QString("%1/.local/opt/com.intel.appup-tablet/%2")
#define APPUP_DIRECTORIES (QStringList() << "featured" << "update")

class MDesktopEntry;
class QFileSystemWatcher;
class QMutex;

class AppUpAppsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int limit READ getLimit WRITE setLimit)
    Q_PROPERTY(int count READ getCount NOTIFY countChanged)
    Q_PROPERTY(AppUpType type READ getType WRITE setType NOTIFY typeChanged)

    Q_ENUMS(Role);
    Q_ENUMS(AppUpType);

public:

    enum Role {
        Type = Qt::UserRole + 1,
        Title = Qt::UserRole + 2,
        Comment = Qt::UserRole + 3,
        Icon = Qt::UserRole + 4,
        Exec = Qt::UserRole + 5,
        Filename = Qt::UserRole + 6
    };

    enum AppUpType {
        Featured = 0,
        Updated,
    };


    explicit AppUpAppsModel(AppUpType type = Featured, QObject *parent = 0);
    ~AppUpAppsModel();
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;



signals:
    void typeChanged();
    void countChanged();

private slots:
    void loadDesktops();

private:
    void setLimit(int limit) {
        mLimit = limit;
    }

    int getLimit() {
        return mLimit;
    }

    int getCount() {
        return mDesktops.count();
    }

    AppUpType getType() {
        return mType;
    }

    void setType(AppUpType type);

    QList<MDesktopEntry *> mDesktops;
    QFileSystemWatcher *mWatcher;
    QMutex mMutex;
    int mLimit;
    AppUpType mType;
    uint mRecurseCount;
    bool mNeedReload;
};

#endif // APPUPAPPSMODEL_H
