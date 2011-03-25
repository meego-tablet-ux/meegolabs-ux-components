/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef DESKTOPDATABASE_H
#define DESKTOPDATABASE_H

#include <QObject>
#include <QHash>
#include <QSettings>
#include <QVariant>

class DesktopDatabase : public QObject
{
    Q_OBJECT
public:
    explicit DesktopDatabase(QObject *parent = 0);
    static DesktopDatabase *instance();

    int getRow(QString filename);
    void setRow(QString filename, int row);

    int getColumn(QString filename);
    void setColumn(QString filename, int column);

    int getPage(QString filename);
    void setPage(QString filename, int page);

private:
    QSettings *settings;
    static DesktopDatabase *desktopDatabaseInstance;
    QHash<QString, QVariant> db;
};

#endif // DESKTOPDATABASE_H
