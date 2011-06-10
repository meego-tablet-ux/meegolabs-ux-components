/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGOTHEMEDIMAGEPROVIDER_H
#define MEEGOTHEMEDIMAGEPROVIDER_H

#include <QDeclarativeImageProvider>

class MGConfItem;

class MeeGoThemedImageProvider : public QDeclarativeImageProvider
{
public:
    MeeGoThemedImageProvider();
    ~MeeGoThemedImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

private:
    MGConfItem *themeItem;
};

#endif // MEEGOTHEMEDIMAGEPROVIDER_H
