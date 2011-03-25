/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef WINDOWLISTENER_H
#define WINDOWLISTENER_H

#include <QObject>
#include <X11/X.h>

class WindowElement;
class WindowInfo;

class WindowListener : public QObject
{
    Q_OBJECT
public:
    WindowListener(QObject *parent = 0);
    ~WindowListener();

    static WindowListener *instance();

    int count() {
        return items.count();
    }

    WindowElement *elementAt(int index) {
        if (index < 0 || index > items.length() - 1)
            return NULL;
        return items[index];
    }

signals:
    void updated();

public slots:
    void windowListUpdated(const QList<WindowInfo> &windowList);

private:
    QList<WindowElement *> items;
    Atom closeWindowAtom;
    Atom activeWindowAtom;

    static WindowListener *windowListenerInstance;
};

#endif // WINDOWLISTENER_H
