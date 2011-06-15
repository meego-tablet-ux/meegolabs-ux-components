/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "plugin.h"
#include "../../lib/locale/localehelper.h"

QML_DECLARE_TYPE(LocaleHelper);

void MeeGoUxLabsLocalePlugin::registerTypes(const char *uri)
{
    qmlRegisterType<LocaleHelper>(uri, 0, 0, "LocaleHelper");
}

void MeeGoUxLabsLocalePlugin::initializeEngine(QDeclarativeEngine *engine, const char *uri)
{
    Q_UNUSED(engine);
    Q_UNUSED(uri);
}

Q_EXPORT_PLUGIN(MeeGoUxLabsLocalePlugin);
