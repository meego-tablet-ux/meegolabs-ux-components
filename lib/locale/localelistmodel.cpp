/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "localelistmodel.h"
#include "meegolocale.h"
#include <QDir>

namespace
{

const QString localeInstallRoot = "/usr/share/qt4/translations";
const QString localeNameFilter = "meego-ux-settings_*.qm";   // assume exactly one '*'


void parseInstalledLocales (QStringList & locales)
{
    const QDir path(localeInstallRoot, localeNameFilter, QDir::NoSort, QDir::Files);
    const QStringList files = path.entryList();

    const int asteriskPos = localeNameFilter.indexOf('*');
    foreach (const QString & file, files) {
        const int localeLength = file.length() - localeNameFilter.length() + 1;
        const QString locale = file.mid(asteriskPos, localeLength);
        locales << locale;
    }
}


void generateDisplayLocales (const QStringList & locales, QStringList & displayLocales)
{
    foreach (const QString & locale, locales) {
        displayLocales << meego::Locale::localeDisplayName(locale);
    }
}

} //anonymous namespace

namespace meego
{

LocaleListModel::LocaleListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles.insert(LocaleRole, "locale");
    roles.insert(LocaleDisplayRole, "displayLocale");
    setRoleNames(roles);

    parseInstalledLocales(mLocales);
    generateDisplayLocales(mLocales, mDisplayLocales);
}

LocaleListModel::~LocaleListModel()
{
}

QVariant LocaleListModel::getData(int index, int role) const
{
    return data(this->index(index), role);
}

int LocaleListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mLocales.size();
}

QVariant LocaleListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() > rowCount()) {
        return QVariant();
    }

    switch (role)
    {
    case LocaleRole:
        return mLocales[index.row()];
        break;
    case LocaleDisplayRole:
        return mDisplayLocales[index.row()];
        break;
    default:
        return QVariant();
    };
}


bool LocaleListModel::contains(const QString & locale) const
{
    return mLocales.contains(locale);
}


int LocaleListModel::indexOf(const QString & locale) const
{
    return mLocales.indexOf(locale);
}


QStringList LocaleListModel::locales() const
{
    return mLocales;
}


QStringList LocaleListModel::displayLocales() const
{
    return mDisplayLocales;
}


}; //namespace meego
