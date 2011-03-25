/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef GESTUREAREA_H
#define GESTUREAREA_H

#include <QDeclarativeItem>
#include <QGesture>

class GestureArea: public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(QGesture *gesture READ gesture);
  //  Q_PROPERTY(QGesture *pangesture READ pangesture)

public:

    GestureArea(QDeclarativeItem * parent = 0);
    QGesture *gesture() const;

signals:
    void pinchGesture();
    void panGesture();
    void gestureStarted();
    void gestureEnded();
protected:
    bool sceneEvent(QEvent *event);

private:
    QGesture * m_gesture;
};

#endif
