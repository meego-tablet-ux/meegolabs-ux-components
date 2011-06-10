/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QFile>
#include <QPixmap>
#include <QIcon>
#include <QString>
#include <QSize>

#include "windowiconprovider.h"
#include "windowelement.h"

WindowIconProvider::WindowIconProvider() :
        QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
{
    listener = WindowListener::instance();
}

WindowIconProvider::~WindowIconProvider()
{
}

QPixmap WindowIconProvider::requestPixmap(const QString &id, QSize *size, const QSize &)
{
    QStringList args = id.split('/');
    for (int i = 0; i < listener->count(); i++)
    {
        WindowElement *e = listener->elementAt(i);
        if (e->windowId() == args[1].toDouble())
        {
            if (args[0] == "notify")
            {
                if (size)
                    *size = e->notify().size();
                return e->notify();
            }
            else
            {
                if (size)
                    *size = e->icon().size();
                return e->icon();
            }
        }
    }

    // Create default pixmap?
    if (size)
        *size = QSize(0, 0);
    return QPixmap();
}
