/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "applicationsmodel.h"
#include "appupappsmodel.h"
#include "backgroundmodel.h"
#include "browserlistmodel.h"
#include "components.h"
#include "devicemodel.h"
#include "desktop.h"
#include "favoriteapplicationsmodel.h"
#include "gconfitem.h"
#include "imageextension.h"
#include "librarymodel.h"
#include "localehelper.h"
#include "meegothemedimageprovider.h"
#include "qmldebugtools.h"
#include "paintspy.h"
#include "recentapplicationsmodel.h"
#include "relativetimestamp.h"
#include "roundeditem.h"
#include "speechbubbledialog.h"
#include "stricturl.h"
#include "systemiconprovider.h"
#include "themedimageprovider.h"
#include "timezonelistmodel.h"
#include "translator.h"
#include "valuespacepublisher.h"
#include "valuespacesubscriber.h"
#include "windowelement.h"
#include "windowiconprovider.h"
#include "windowmodel.h"

#include <MNotification>
#include <mnotificationgroup.h>

void components::registerTypes(const char *uri)
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

    qmlRegisterType<ImageExtension>(uri, 0,0, "ImageExtension");

    qmlRegisterType<ValueSpacePublisher>(uri, 0,0, "ValueSpacePublisher");
    qmlRegisterType<ValueSpaceSubscriber>(uri, 0,0,"ValueSpaceSubscriber");

    qmlRegisterType<RelativeTimeStamp>(uri, 0,1, "RelativeTimeStamp");
        
    qmlRegisterType<speechbubbledialog>(uri, 0, 1, "RectangularBubble");

    qmlRegisterType<RoundedItem>(uri,0,1,"RoundedItem");

    qmlRegisterType<BrowserItemListModel>(uri,0,0,"BrowserItemListModel");
    qmlRegisterType<Translator>(uri,0,0,"Translator");
    qmlRegisterType<QmlDebugTools>(uri, 0, 0, "QmlDebugTools");

    qmlRegisterType<BackgroundModel>(uri, 0, 1, "BackgroundModel");
    qmlRegisterType<PaintSpy>(uri, 0, 1, "PaintSpy");

    // locale stuff
    qmlRegisterType<LocaleHelper>(uri, 0, 1, "LocaleHelper");

    // notifications
    qmlRegisterInterface<MNotification>("Notification");
    qmlRegisterInterface<MNotificationGroup>("NotificationGroup");

    qmlRegisterType<GConfItem>(uri, 0, 1, "GConfItem");

    // qt bug workarounds
    qmlRegisterType<StrictUrl>(uri, 0, 1, "StrictUrl");
}

void components::initializeEngine(QDeclarativeEngine *engine, const char *uri)
{
    Q_UNUSED(uri);
    engine->addImageProvider("systemicon", new SystemIconProvider);
    engine->addImageProvider("windowicon", new WindowIconProvider);
    engine->addImageProvider("theme", new ThemedImageProvider);
    engine->addImageProvider("meegotheme", new MeeGoThemedImageProvider);
}

QML_DECLARE_TYPE(MNotification);
QML_DECLARE_TYPE(MNotificationGroup);
Q_EXPORT_PLUGIN(components);
