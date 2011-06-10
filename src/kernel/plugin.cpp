#include "plugin.h"

#include "desktop.h"
#include "desktopdatabase.h"
#include "gconfitem.h"
#include "imageextension.h"
#include "meegothemedimageprovider.h"
#include "paintspy.h"
#include "qmldebugtools.h"
#include "relativetimestamp.h"
#include "roundeditem.h"
#include "speechbubbledialog.h"
#include "stricturl.h"
#include "systemiconprovider.h"
#include "timestampupdater.h"
#include "translator.h"
#include "udisk_interface.h"
#include "valuespacepublisher.h"
#include "valuespacesubscriber.h"
#include "windowelement.h"
#include "windowiconprovider.h"
#include "windowlistener.h"
#include "windowinfo.h"

void MeeGoUxComponentsKernelPlugin::registerTypes(const char *uri)
{

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
Q_EXPORT_PLUGIN(MeeGoUxComponentsKernelPlugin);
