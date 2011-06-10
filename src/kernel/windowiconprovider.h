/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef WINDOWWINDOWPROVIDER_H
#define WINDOWWINDOWPROVIDER_H

#include <QDeclarativeImageProvider>
#include "windowlistener.h"

class WindowIconProvider : public QDeclarativeImageProvider
{
public:
    WindowIconProvider();
    ~WindowIconProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

private:
    WindowListener *listener;
};

#endif // WINDOWWINDOWPROVIDER_H
