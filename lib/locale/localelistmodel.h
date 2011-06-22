/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef LOCALE_LIST_MODEL_H
#define LOCALE_LIST_MODEL_H

#include "meegolocale_global.h"
#include <QObject>
#include <QStringList>
#include <QAbstractListModel>


namespace meego
{

class LOCALESHARED_EXPORT LocaleListModel: public QAbstractListModel
{
    Q_OBJECT;
    Q_PROPERTY(int count READ rowCount);

public:

    LocaleListModel(QObject *parent = 0);
    ~LocaleListModel();

    enum Role {
        LocaleRole = Qt::UserRole + 1,
        LocaleDisplayRole
    };
    Q_ENUMS(Role);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE QVariant getData(int index, int role) const;

    Q_INVOKABLE bool contains(const QString & locale) const;
    Q_INVOKABLE int indexOf(const QString & locale) const;

    Q_INVOKABLE QStringList locales() const;
    Q_INVOKABLE QStringList displayLocales() const;

protected:

    QStringList mLocales;
    QStringList mDisplayLocales;
};

} //namespace meego

#endif // LOCALE_LIST_MODEL_H
