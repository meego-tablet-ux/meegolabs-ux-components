/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QProcess>
#include <QtDeclarative/qdeclarative.h>
#include <QCryptographicHash>
#include <QFile>

#include "desktop.h"
#include "desktopdatabase.h"

#define WIDTH_KEY "Desktop Entry/X-MEEGO-APP-HOME-WIDTH"
#define HEIGHT_KEY "Desktop Entry/X-MEEGO-APP-HOME-HEIGHT"
#define PRIORITY_KEY "Desktop Entry/X-MEEGO-APP-HOME-PRIORITY"
#define ROW_KEY "Desktop Entry/X-MEEGO-APP-HOME-ROW"
#define COLUMN_KEY "Desktop Entry/X-MEEGO-APP-HOME-COLUMN"
#define PAGE_KEY "Desktop Entry/X-MEEGO-APP-HOME-PAGE"

Desktop::Desktop(const QString &fileName, QObject *parent):
    QObject(parent),
    m_filename(fileName),
    m_entry(new MDesktopEntry(fileName)),
    m_assigned(false)
{
    db = DesktopDatabase::instance();

    if (contains(WIDTH_KEY))
        m_width = value(WIDTH_KEY).toInt();
    else
        m_width = 1;

    if (contains(HEIGHT_KEY))
        m_height = value(HEIGHT_KEY).toInt();
    else
        m_height = 1;

    if (contains(PRIORITY_KEY))
        m_priority = value(PRIORITY_KEY).toInt();
    else
        m_priority = 0;

    m_row = db->getRow(m_filename);
    if (m_row == -1 && contains(ROW_KEY))
    {
        m_row = value(ROW_KEY).toInt();
    }

    m_column = db->getColumn(m_filename);
    if (m_column == -1 && contains(COLUMN_KEY))
    {
        m_column = value(COLUMN_KEY).toInt();
    }

    m_page = db->getPage(m_filename);
    if (m_page == -1 && contains(PAGE_KEY))
    {
        m_page = value(PAGE_KEY).toInt();
    }

    ///Set the id:
    QFile file(m_filename);
    file.open(QIODevice::ReadOnly);
    if(!file.isOpen()) return;
    QString hash = QCryptographicHash::hash(file.readAll(),QCryptographicHash::Md5);
    file.close();
    m_id = hash;
}

Desktop::~Desktop()
{
    emit aboutToDelete();
}

int Desktop::row()
{
    return m_row;
}

void Desktop::setRow(int row)
{
    m_row = row;
    db->setRow(m_filename, row);
    emit rowChanged();
}

int Desktop::column()
{
    return m_column;
}

void Desktop::setColumn(int column)
{
    m_column = column;
    db->setColumn(m_filename, column);
    emit columnChanged();
}

int Desktop::page()
{
    return m_page;
}

void Desktop::setPage(int page)
{
    m_page = page;
    db->setPage(m_filename, page);
}

int Desktop::priority()
{
    return m_priority;
}

void Desktop::setPriority(int priority)
{
    m_page = priority;
}

QML_DECLARE_TYPE(Desktop);
