/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef SPEECHBUBBLEDIALOG_H
#define SPEECHBUBBLEDIALOG_H

#include <QGraphicsItem>
#include <QPainter>
#include <QRect>
#include <QRectF>
#include <QDeclarativeItem>

class speechbubbledialog : public QDeclarativeItem
 {
   Q_OBJECT
   Q_PROPERTY( int offset READ offset WRITE setoffset )
   //        NOTIFY geometryChanged )

  public:
     speechbubbledialog();
     ~speechbubbledialog();

     QRectF boundingRect() const;

     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget);

    void geometryChanged ( const QRectF & newGeometry, const QRectF & oldGeometry);



    qint16 radius() { return m_radius; }
    void setRadius(qint16 value) { m_radius= value; }

    qint16 offset() { return m_offset; }
    void setoffset(qint16 value) { m_offset= value; }



 private:
     QRect*  referenceRect;
     QRectF* referenceRectF;

     int x;
     int y;

     int m_offset;

     int height;
     int width;
     int m_radius;

     QColor color;

 };
#endif // SPEECHBUBBLEDIALOG_H
