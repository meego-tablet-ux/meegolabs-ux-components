/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QtDeclarative/qdeclarative.h>
#include <QHash>
#include <QX11Info>
#include "windowmodel.h"
#include "windowelement.h"
#include "windowinfo.h"

WindowModel::WindowModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles.insert(WindowElement::Title, "title");
    roles.insert(WindowElement::Id, "windowId");
    roles.insert(WindowElement::Icon, "icon");
    roles.insert(WindowElement::Notify, "notify");
    roles.insert(WindowElement::HasNotify, "hasnotify");
    setRoleNames(roles);

    // Get the X11 Atoms for closing and activating a window
    closeWindowAtom  = XInternAtom(QX11Info::display(), "_NET_CLOSE_WINDOW",  false);
    activeWindowAtom = XInternAtom(QX11Info::display(), "_NET_ACTIVE_WINDOW", false);

    listener = WindowListener::instance();
    connect(listener, SIGNAL(updated()), this, SLOT(update()));
}

int WindowModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return listener->count();
}

QVariant WindowModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() > listener->count())
        return QVariant();

    WindowElement *e = listener->elementAt(index.row());
    if (role == WindowElement::Title)
        return e->title();

    if (role == WindowElement::Id)
        return e->windowId();

    if (role == WindowElement::Notify)
        return QString("image://windowicon/notify/%1").arg(e->windowId());

    if (role == WindowElement::HasNotify)
        return e->hasNotify();

    return QString("image://windowicon/app/%1").arg(e->windowId());
}

void WindowModel::update()
{
    emit modelReset();
}

void WindowModel::close(int windowId)
{
    XEvent ev;
    memset(&ev, 0, sizeof(ev));

    Display *display = QX11Info::display();

    Window rootWin = QX11Info::appRootWindow(QX11Info::appScreen());

    ev.xclient.type         = ClientMessage;
    ev.xclient.window       = windowId;
    ev.xclient.message_type = closeWindowAtom;
    ev.xclient.format       = 32;
    ev.xclient.data.l[0]    = CurrentTime;
    ev.xclient.data.l[1]    = rootWin;

    XSendEvent(display,
               rootWin, False,
               SubstructureRedirectMask, &ev);
}

void WindowModel::raise(int windowId)
{
    XEvent ev;
    memset(&ev, 0, sizeof(ev));

    Display *display = QX11Info::display();

    Window rootWin = QX11Info::appRootWindow(QX11Info::appScreen());

    ev.xclient.type         = ClientMessage;
    ev.xclient.window       = windowId;
    ev.xclient.message_type = activeWindowAtom;
    ev.xclient.format       = 32;
    ev.xclient.data.l[0]    = 1;
    ev.xclient.data.l[1]    = CurrentTime;
    ev.xclient.data.l[2]    = 0;

    XSendEvent(display,
               rootWin, False,
               SubstructureRedirectMask, &ev);
}

QML_DECLARE_TYPE(WindowModel);
