/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef QMLDEBUGTOOLS_H
#define QMLDEBUGTOOLS_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class QmlDebugTools : public QObject
{
    Q_OBJECT
public:

    explicit QmlDebugTools();
    Q_INVOKABLE void writetofile( const QString& ,const QString& );


};

#endif // QMLDEBUGTOOLS_H
