/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "qmldebugtools.h"
#include <QDebug>
#include <QString>


QmlDebugTools::QmlDebugTools():
        QObject()
{


}

void QmlDebugTools::writetofile(const QString &nameofFile,
                                const QString &message)
{

    QFile aFile(nameofFile);

    if ( aFile.open(QIODevice::WriteOnly | QIODevice::Append) )
    {
         aFile.write( message.toUtf8());
         aFile.write( "\n");
    }
    aFile.close();
}
