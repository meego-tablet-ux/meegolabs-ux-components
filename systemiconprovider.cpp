/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QApplication>
#include <QFile>
#include <QPixmap>
#include <QIcon>
#include <QString>
#include <QSize>
#include <QDesktopWidget>
#include <QDebug>
#include <mgconfitem.h>


#include "systemiconprovider.h"

#define THEME_KEY "/meego/ux/theme"

SystemIconProvider::SystemIconProvider() :
        QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
{
    themeItem = new MGConfItem(THEME_KEY);
    if (themeItem->value().isNull() ||
            !QFile::exists(QString("/usr/share/themes/") + themeItem->value().toString()))
    {
        QRect screenRect = qApp->desktop()->screenGeometry();

        // TODO: Check both the resolution and the DPI to determine the default
        //       theme location

        if (screenRect.width() == 1024 && screenRect.height() == 600)
            themeItem->set("1024-600-10");
        else if (screenRect.width() == 1280 && screenRect.height() == 800)
            themeItem->set("1280-800-7");
        else
            // fallback to something...
            themeItem->set("1024-600-10");
    }
}

SystemIconProvider::~SystemIconProvider()
{
    delete themeItem;
}

QPixmap SystemIconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    int width = requestedSize.width() > 0 ? requestedSize.width() : 100;
    int height = requestedSize.height() > 0 ? requestedSize.height() : 100;

    if (size)
    {
        *size = QSize(width, height);
    }

    if (id.startsWith('/'))
    {
        if (QFile::exists(id))
        {
            if(requestedSize.width() < 0 && requestedSize.height() < 0)
            {
                QPixmap pixmap(id);
                if(size)
                    *size = QSize(pixmap.width(),pixmap.height());
                return pixmap;
            }
            return QPixmap(id).scaled(requestedSize);
        }
    }
    else if (QFile::exists(QString("/usr/share/themes/") + themeItem->value().toString() + "/icons/launchers/" + id + ".png"))
    {
        QPixmap pixmap(QString("/usr/share/themes/") + themeItem->value().toString() + "/icons/launchers/" + id + ".png");

        if(requestedSize.width() < 0 && requestedSize.height() < 0)
        {
            if(size)
                *size = QSize(pixmap.width(),pixmap.height());
            return pixmap;
        }

        return pixmap.scaled(requestedSize);
    }
    else if (QFile::exists(QString("/usr/share/themes/") + themeItem->value().toString() + "/icons/settings/" + id + ".png"))
    {
        QPixmap pixmap(QString("/usr/share/themes/") + themeItem->value().toString() + "/icons/settings/" + id + ".png");

        if(requestedSize.width() < 0 && requestedSize.height() < 0)
        {
            if(size)
                *size = QSize(pixmap.width(),pixmap.height());
            return pixmap;
        }

        return pixmap.scaled(requestedSize);
    }
    else if (QIcon::hasThemeIcon(id))
    {
        ///according to docs, this will return a pixmap of at-max, width,height.
        ///However, it may return a smaller pixmap.
        if(size)
            *size = QIcon::fromTheme(id).actualSize(QSize(width, height));
        return QIcon::fromTheme(id).pixmap(QSize(width, height));
    }

    // Default icon?
    return QPixmap(requestedSize);
}
