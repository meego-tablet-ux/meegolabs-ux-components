/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "gesturearea.h"
#include <QEvent>
#include <QGestureEvent>
#include <QtDebug>
GestureArea::GestureArea(QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
{
    setAcceptTouchEvents(true);
    grabGesture(Qt::PinchGesture);
}

QGesture* GestureArea::gesture() const
{
    return m_gesture;
}


bool GestureArea::sceneEvent(QEvent *event)
{
    if (event->type() == QEvent::TouchBegin ||
        event->type() == QEvent::TouchEnd ||
        event->type() == QEvent::TouchUpdate) 
    {
        return true;
    }
    if (event->type() == QEvent::Gesture) {
        QGestureEvent * ge = static_cast<QGestureEvent*>(event);
        if (ge->gesture(Qt::PinchGesture) ) {
            m_gesture = ge->gesture(Qt::PinchGesture);
            Qt::GestureState state= m_gesture->state();
            if (state == Qt::GestureStarted) {
                emit gestureStarted();
            }else if (state == Qt::GestureCanceled || state == Qt::GestureFinished)
                emit gestureEnded();
            emit pinchGesture();
        }else if (ge->gesture(Qt::PanGesture)) {
            m_gesture = ge->gesture(Qt::PanGesture);
            Qt::GestureState state= m_gesture->state();
            if (state == Qt::GestureStarted) {
                emit gestureStarted();
            }else if (state == Qt::GestureCanceled || state == Qt::GestureFinished)
                emit gestureEnded();
            emit panGesture();
        }
            return true;
    }
    return QDeclarativeItem::sceneEvent(event);
}

