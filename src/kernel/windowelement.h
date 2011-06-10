/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef WINDOWELEMENT_H
#define WINDOWELEMENT_H

#include <QDebug>
#include <QObject>
#include <QPixmap>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

#include "windowinfo.h"

class WindowElement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title);
    Q_PROPERTY(int windowId READ windowId);
    Q_PROPERTY(QPixmap icon READ icon);
    Q_PROPERTY(QPixmap notify READ notify);
    Q_PROPERTY(bool hasNotify READ hasNotify);

public:
    explicit WindowElement(WindowInfo info, QObject *parent = 0);

    enum Role{ Title = Qt::UserRole + 1,
               Id = Qt::UserRole + 2,
               Icon = Qt::UserRole + 4,
               Notify = Qt::UserRole + 5,
               HasNotify = Qt::UserRole + 6
    };

    QString title() const {
        return m_title;
    }

    int windowId() {
        return m_windowId;
    }

    QPixmap icon() const {
        return m_pixmap;
    }

    QPixmap notify() const {
        return m_notify;
    }

    bool hasNotify() {
        return !m_notify.isNull();
    }

private:
    QString m_title;
    int m_windowId;
    QPixmap m_pixmap;
    QPixmap m_notify;
};

#endif // WINDOWELEMENT_H
