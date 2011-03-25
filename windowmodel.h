/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef WINDOWMODEL_H
#define WINDOWMODEL_H

#include <QAbstractListModel>
#include <X11/X.h>

#include "windowlistener.h"

class WindowElement;
class WindowInfo;

class WindowModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY (int count READ count NOTIFY modelReset)

public:
    explicit WindowModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    int count() {
        return listener->count();
    }

signals:
    void modelReset();

public slots:
    void close(int windowId);
    void raise(int windowId);

private slots:
    void update();

private:
    Atom closeWindowAtom;
    Atom activeWindowAtom;
    WindowListener *listener;
};

#endif // WINDOWMODEL_H
