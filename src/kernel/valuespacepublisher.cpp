/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QValueSpacePublisher>
#include "valuespacepublisher.h"


ValueSpacePublisher::ValueSpacePublisher(QObject *parent):
    QObject(parent) ,
    m_publisher(0)
{
    // should move this to a global place, so we only initialize it once
    // and when close the publisher, the subscriber will not crash
    QValueSpace::initValueSpaceServer();
}

ValueSpacePublisher::~ValueSpacePublisher()
{
    delete m_publisher;
}

void ValueSpacePublisher::setPath(const QString path)
{
    QString tpath = path.trimmed();
    if (m_path == tpath) {
        return;
    }

    m_path = tpath;

    delete m_publisher;

    m_publisher = new QValueSpacePublisher(m_path);

    emit pathChanged();
}


void ValueSpacePublisher::setValue(const QString &name, const QVariant &data)
{
    if (m_publisher) {
        m_publisher->setValue(name, data);
    }
}
