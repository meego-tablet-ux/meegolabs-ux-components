/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef THEMEDIMAGEPROVIDER_H
#define THEMEDIMAGEPROVIDER_H

#include <QDeclarativeImageProvider>

class MGConfItem;

class ThemedImageProvider : public QDeclarativeImageProvider
{
public:
    ThemedImageProvider();
    ~ThemedImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

private:
    MGConfItem *themeItem;
};

#endif // THEMEDIMAGEPROVIDER_H
