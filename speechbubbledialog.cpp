/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "speechbubbledialog.h"
#include <QPainter>


speechbubbledialog::speechbubbledialog()
    :QDeclarativeItem(),x(0),y(0),m_offset(5),m_radius(5),
    color( QColor(29,29,29,255) )
{
    setFlag( QGraphicsItem::ItemHasNoContents, false );
    referenceRect = new QRect();
}

speechbubbledialog::~speechbubbledialog()
{
    delete referenceRect;
}

QRectF speechbubbledialog::boundingRect() const
{  
    return QRectF( x - m_offset, y, property("width").toInt() + m_offset, property("height").toInt());
}


void speechbubbledialog::geometryChanged ( const QRectF & newGeometry, const QRectF & oldGeometry)
{

    referenceRect->setRect( x, y, newGeometry.width(), newGeometry.height());

    QDeclarativeItem::geometryChanged ( newGeometry, oldGeometry );
}

void speechbubbledialog::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                               QWidget *widget)
{

    Q_UNUSED(option)
    Q_UNUSED(widget)


    QLinearGradient linearGrad(QPointF(x, y),
                               QPointF(property("width").toInt(),
                                       property("height").toInt()));

    QPainterPath path;
    QRect rect= *referenceRect;
    int diam = 2 * m_radius;

    int x1, y1, x2, y2;
    rect.getCoords(&x1, &y1, &x2, &y2);
    path.moveTo(x2, y1 + m_radius);
    path.arcTo(QRect(x2 - diam, y1, diam, diam), 0.0, +90.0);
    path.lineTo(x1 + m_radius, y1);
    path.arcTo(QRect(x1, y1, diam, diam), 90.0, +90.0);



    path.lineTo( x1,
                 property("height").toInt()/2 - m_offset );


    path.lineTo(x1-m_offset,
                property("height").toInt()/2);


    path.lineTo(x1,
                m_offset + property("height").toInt()/2);

    path.lineTo(x1, y2 - m_radius);

    path.arcTo(QRect(x1, y2 - diam, diam, diam), 180.0, +90.0);
    path.lineTo(x1 + m_radius, y2);
    path.arcTo(QRect(x2 - diam, y2 - diam, diam, diam), 270.0, +90.0);
    path.closeSubpath();


    painter->setPen(QPen(QColor(118, 117, 117), 2, Qt::SolidLine,
                         Qt::FlatCap, Qt::MiterJoin));

    painter->setBrush( color );
    painter->drawPath(path);

}
