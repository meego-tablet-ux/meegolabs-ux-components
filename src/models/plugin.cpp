/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "plugin.h"
#include "applicationsmodel.h"
#include "appupappsmodel.h"
#include "backgroundmodel.h"
#include "browserlistmodel.h"
#include "devicemodel.h"
#include "favoriteapplicationsmodel.h"
#include "librarymodel.h"
#include "recentapplicationsmodel.h"
#include "timezonelistmodel.h"
#include "windowmodel.h"

void MeeGoUxLabsModelsPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<TimezoneListModel>(uri, 0, 0, "TimezoneListModel");
    qmlRegisterType<RecentApplicationsModel>(uri,0,1, "RecentApplicationsModel");


    qmlRegisterType<AppUpAppsModel>(uri, 0, 1, "AppUpAppsModel");
    qmlRegisterType<FavoriteApplicationsModel>(uri,0,1, "FavoriteApplicationsModel");
    qmlRegisterType<ApplicationsModel>(uri, 0, 1, "ApplicationsModel");
    qmlRegisterType<WindowModel>(uri, 0, 1, "WindowModel");

    qmlRegisterInterface<Desktop>("Desktop");
    qmlRegisterInterface<WindowElement>("WindowElement");
    qmlRegisterType<devicemodel>(uri,0,1,"UDiskDeviceModel");

    qmlRegisterType<BrowserItemListModel>(uri,0,0,"BrowserItemListModel");

    qmlRegisterType<BackgroundModel>(uri, 0, 1, "BackgroundModel");
  }

void MeeGoUxLabsModelsPlugin::initializeEngine(QDeclarativeEngine *engine, const char *uri)
{
    Q_UNUSED(uri);
}

Q_EXPORT_PLUGIN(MeeGoUxLabsModelsPlugin);
