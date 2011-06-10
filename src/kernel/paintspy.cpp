/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QDebug>

#include <QTimer>
#include <QImage>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QDateTime>

#include "paintspy.h"

PaintSpy::PaintSpy(QDeclarativeItem *parent): QDeclarativeItem(parent)
{
    m_delayMS = 0;
    m_timer = new QTimer;
    m_window = QApplication::activeWindow();

    if (m_window)
        connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    else
        qWarning() << "paintspy: no active window found";
}

PaintSpy::~PaintSpy()
{
    delete m_timer;
}

//
// public methods
//

QRectF PaintSpy::rect() {
    return m_rect;
}

void PaintSpy::setRect(const QRectF &rect)
{
    m_rect = rect;
}

int PaintSpy::delayMS()
{
    return m_delayMS;
}

void PaintSpy::setDelayMS(int delayMS)
{
    if (delayMS == m_delayMS)
        return;

    m_timer->stop();
    m_delayMS = delayMS;
    if (delayMS > 0)
        m_timer->setInterval(delayMS);
    m_timer->start();
}

void PaintSpy::setRegion(int x, int y, int width, int height)
{
    m_rect.setRect(x, y, width, height);
}

QString PaintSpy::saveFile(const QString &path, int width, int height,
                           const QStringList &list)
{
    if (list.size() < 4) {
        qWarning() << "expected four colors in PaintSpy::saveFile";
        return QString();
    }

    QString filepath = path;
    if (filepath.startsWith("~/")) {
        filepath.remove(0, 1);
        filepath.prepend(QDir::homePath());
    }

    if (!filepath.endsWith("/"))
        filepath.append("/");
    filepath.append("colorbars-");

    // create a unique filename
    QDateTime dt = QDateTime::currentDateTime();
    filepath.append(dt.toString("yyyyMMdd-hhmmss-zzz"));
    filepath.append(".png");

    // create a tiny four-pixel image with the right colors
    QImage image(4, 1, QImage::Format_RGB32);
    for (int i = 0; i < 4; i++)
        image.setPixel(i, 0, QColor(list[i]).rgb());

    // scale and save the image
    image.scaled(width, height).save(filepath, "PNG");

    return filepath;
}

//
// protected slots
//

void PaintSpy::timeout()
{
    // choose points from centers of three rows and columns
    QRect rect = m_rect.toRect();
    int avg = (rect.width() + rect.height()) / 2;
    int trim = avg / 6;
    rect.setLeft(rect.left() + trim);
    rect.setRight(rect.right() - trim);
    rect.setTop(rect.top() + trim);
    rect.setBottom(rect.bottom() - trim);

    // TODO: account for orientation

    QImage image = QPixmap::grabWidget(m_window, rect).toImage();
    int w = image.width();
    int h = image.height();
    if (w == 0 || h == 0)
        return;

    int x[3] = { 1, w / 2, w - 2};
    int y[3] = { 1, h / 2, h - 2};

    // return a list of the nine pixels, first row 0, then row 1, ...
    QList<QColor> list;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            list.append(QColor(image.pixel(x[j], y[i])));

    // find average color
    int red = 0;
    int green = 0;
    int blue = 0;
    for (int i = 0; i < 9; i++) {
        red += list[i].red();
        green += list[i].green();
        blue += list[i].blue();
    }
    QColor avgColor(red / 9, green / 9, blue / 9);

#if 0
    static int count = 1;
    QFile file(QString("image") + QString::number(count++) + ".jpg");
    image.save(&file, "jpg");
#endif

    // generate three more colors with the same hue
    QStringList clist;
    clist.append(avgColor.name());
    clist.append(QColor::fromHsv(avgColor.hue(), 128, 64).name());
    clist.append(QColor::fromHsv(avgColor.hue(), 255, 128).name());
    clist.append(QColor::fromHsv(avgColor.hue(), 128, 192).name());
    emit colorsCaptured(clist);
}

