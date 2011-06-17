/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "plugin.h"
#include "meegolocale.h"
#include "localelistmodel.h"

//QML_DECLARE_TYPE(meego::Locale);
//QML_DECLARE_TYPE(meego::LocaleListModel);

void MeeGoUxLabsLocalePlugin::registerTypes(const char *uri)
{
    qmlRegisterType<meego::Locale>(uri, 0, 1, "LocaleHelper");
    qmlRegisterType<meego::LocaleListModel>(uri, 0, 1, "LocaleListModel");
}

void MeeGoUxLabsLocalePlugin::initializeEngine(QDeclarativeEngine *engine, const char *uri)
{
    Q_UNUSED(engine);
    Q_UNUSED(uri);
}

Q_EXPORT_PLUGIN(MeeGoUxLabsLocalePlugin);
