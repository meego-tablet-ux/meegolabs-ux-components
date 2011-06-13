/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef KERNEL_H
#define KERNEL_H

#include <QDeclarativeExtensionPlugin>

class MeeGoUxComponentsKernelPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    void registerTypes(const char *uri);
    void initializeEngine(QDeclarativeEngine *engine, const char *uri);
};

#endif // KERNEL_H
