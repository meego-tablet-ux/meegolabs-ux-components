/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef __paintspy_h
#define __paintspy_h

#include <QDeclarativeItem>

//
// PaintSpy periodically captures an image of its child and provides the data
//   in a QImage through the imageCaptured signal
//   The rect property provides the rectangle of interest
//   The delayMS property provides the capture interval, 0 = never (default)
//

class PaintSpy: public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(QRectF rect READ rect WRITE setRect)
    Q_PROPERTY(int delayMS READ delayMS WRITE setDelayMS)

public:

    PaintSpy(QDeclarativeItem *parent = NULL);
    virtual ~PaintSpy();

    QRectF rect();
    void setRect(const QRectF& rect);

    int delayMS();
    void setDelayMS(int delayMS);

    Q_INVOKABLE void setRegion(int x, int y, int width, int height);
    Q_INVOKABLE QString saveFile(const QString &path, int width, int height,
                                 const QStringList &list);

signals:
    void colorsCaptured(const QStringList &list);

protected slots:
    void timeout();

private:
    QRectF m_rect;
    int m_delayMS;

    QTimer *m_timer;
    QWidget *m_window;
};

#endif
