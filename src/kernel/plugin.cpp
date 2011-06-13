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
#include "desktop.h"
#include "desktopdatabase.h"
#include "devicemodel.h"
#include "favoriteapplicationsmodel.h"
#include "gconfitem.h"
#include "imageextension.h"
#include "librarymodel.h"
#include "meegothemedimageprovider.h"
#include "paintspy.h"
#include "plugin.h"
#include "qmldebugtools.h"
#include "recentapplicationsmodel.h"
#include "relativetimestamp.h"
#include "roundeditem.h"
#include "speechbubbledialog.h"
#include "stricturl.h"
#include "systemiconprovider.h"
#include "timestampupdater.h"
#include "timezonelistmodel.h"
#include "translator.h"
#include "udisk_interface.h"
#include "valuespacepublisher.h"
#include "valuespacesubscriber.h"
#include "windowelement.h"
#include "windowiconprovider.h"
#include "windowinfo.h"
#include "windowlistener.h"
#include "windowmodel.h"


void MeeGoUxComponentsKernelPlugin::registerTypes(const char *uri)
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

    qmlRegisterInterface<Desktop>("Desktop");
    qmlRegisterInterface<WindowElement>("WindowElement");

    qmlRegisterType<ImageExtension>(uri, 0,0, "ImageExtension");

    qmlRegisterType<ValueSpacePublisher>(uri, 0,0, "ValueSpacePublisher");
    qmlRegisterType<ValueSpaceSubscriber>(uri, 0,0,"ValueSpaceSubscriber");

    qmlRegisterType<RelativeTimeStamp>(uri, 0,1, "RelativeTimeStamp");

    qmlRegisterType<speechbubbledialog>(uri, 0, 1, "RectangularBubble");

    qmlRegisterType<RoundedItem>(uri,0,1,"RoundedItem");

    qmlRegisterType<Translator>(uri,0,0,"Translator");
    qmlRegisterType<QmlDebugTools>(uri, 0, 0, "QmlDebugTools");

    qmlRegisterType<PaintSpy>(uri, 0, 1, "PaintSpy");

    qmlRegisterType<GConfItem>(uri, 0, 1, "GConfItem");

    // qt bug workarounds
    qmlRegisterType<StrictUrl>(uri, 0, 1, "StrictUrl");
}


void MeeGoUxComponentsKernelPlugin::initializeEngine(QDeclarativeEngine *engine, const char *uri)
{
    Q_UNUSED(uri);
}


Q_EXPORT_PLUGIN(MeeGoUxComponentsKernelPlugin);
