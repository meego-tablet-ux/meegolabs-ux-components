/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef SYSTEMICONPROVIDER_H
#define SYSTEMICONPROVIDER_H

#include <QDeclarativeImageProvider>

class MGConfItem;

class SystemIconProvider : public QDeclarativeImageProvider
{
public:
    SystemIconProvider();
    ~SystemIconProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

private:
    MGConfItem *themeItem;
};

#endif // SYSTEMICONPROVIDER_H
