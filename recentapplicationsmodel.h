/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef RECENTAPPLICATIONSMODEL_H
#define RECENTAPPLICATIONSMODEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QObject>
#include <QStringList>

#include "desktop.h"

class MGConfItem;

class RecentApplicationsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int limit READ getLimit WRITE setLimit)

public:
    explicit RecentApplicationsModel(QObject *parent = 0);
    ~RecentApplicationsModel();
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    bool contains(Desktop *target) {
        return m_recents.contains(target);
    }

    void append(QString filename);

    Q_INVOKABLE bool isEmpty() {
        return m_recents.isEmpty();
    }

    Q_INVOKABLE void remove(QString filename);

    Desktop *takeFirst() {
        return m_recents.takeFirst();
    }

    void setLimit(int limit) {
        m_limit = limit;
    }
    int getLimit() {
        return m_limit;
    }

signals:
    void modelReset();

private slots:
    void recentAppsChanged();

private:
    void saveRecents();
    QList<Desktop *> m_recents;
    int m_limit;
    MGConfItem *m_gitem;
};

#endif // RECENTAPPLICATIONSMODEL_H
