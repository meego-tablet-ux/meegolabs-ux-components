/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef ROUNDEDITEM_H
#define ROUNDEDITEM_H

#include <QDeclarativeItem>

class RoundedItem : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(double radius READ radius WRITE setRadius NOTIFY radiusChanged)
public:
    explicit RoundedItem(QDeclarativeItem *parent = 0);

    double radius() const;
    void setRadius(double value);

    virtual QPainterPath clipPath() const;
    virtual QPainterPath shape() const;

signals:
    void radiusChanged();

private:
    double mRadius;

};

#endif // ROUNDEDITEM_H
