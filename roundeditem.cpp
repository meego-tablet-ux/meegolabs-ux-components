/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "roundeditem.h"
#include <QDebug>

RoundedItem::RoundedItem(QDeclarativeItem *parent) :
    QDeclarativeItem(parent), mRadius(0)
{
    setClip(true);
}

double RoundedItem::radius() const
{
    return mRadius;
}

void RoundedItem::setRadius(double value)
{
    prepareGeometryChange();
    mRadius = value;
    emit radiusChanged();
    geometryChanged(boundingRect(), boundingRect());
}

QPainterPath RoundedItem::clipPath() const
{
    QPainterPath path;
    path.addRoundedRect(boundingRect(), mRadius, mRadius);

    return path;
}

QPainterPath RoundedItem::shape() const
{
    return clipPath();
}
