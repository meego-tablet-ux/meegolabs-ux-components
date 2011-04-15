#include <QColor>
#include <QFile>
#include <QPainter>
#include <qmath.h>
#include <MGConfItem>

#include "networkindicator.h"
#include <contextsubscriber/contextproperty.h>

NetworkIndicator::NetworkIndicator(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    m_active(false)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);

    m_themeItem = new MGConfItem("/meego/ux/theme");
    connect(m_themeItem, SIGNAL(valueChanged()), this, SLOT(themeUpdated()));

    themeUpdated();
    setActive(true);
}

void NetworkIndicator::setActive(bool active)
{
    if (active == m_active)
        return;

    m_active = active;
    if (m_active)
    {
        m_networkTypeProperty = new ContextProperty("Internet.NetworkType", this);
        connect(m_networkTypeProperty, SIGNAL(valueChanged()), this, SLOT(updateIcon()));

        m_networkStateProperty = new ContextProperty("Internet.NetworkState", this);
        connect(m_networkStateProperty, SIGNAL(valueChanged()), this, SLOT(updateIcon()));

        m_signalStrengthProperty = new ContextProperty("Internet.SignalStrength", this);
        connect(m_signalStrengthProperty, SIGNAL(valueChanged()), this, SLOT(updateIcon()));
    }
    else
    {
        if (m_networkTypeProperty)
        {
            disconnect(m_networkTypeProperty);
            delete m_networkTypeProperty;
            m_networkTypeProperty = 0;
        }

        if (m_networkStateProperty)
        {
            disconnect(m_networkStateProperty);
            delete m_networkStateProperty;
            m_networkStateProperty = 0;
        }

        if (m_signalStrengthProperty)
        {
            disconnect(m_signalStrengthProperty);
            delete m_signalStrengthProperty;
            m_signalStrengthProperty = 0;
        }
    }
}

void NetworkIndicator::updateIcon()
{
    QGraphicsItem::update(boundingRect());
}

void NetworkIndicator::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!m_active)
        return;

    QString networkState = m_networkStateProperty->value().toString();
    if (!networkState.isEmpty() && networkState != "disconnected")
    {
        // TODO: Add support for displaying the operator name while roaming

        int offset = 0;
        int strength = m_signalStrengthProperty->value().toInt();
        QString networkType = m_networkTypeProperty->value().toString();

        if (networkType != "ethernet")
        {
           int index = strength < 0 || strength > 100 ? 0 : qCeil(((double)strength/100.0) * 4.0);
	   p->drawPixmap(0, 0, m_signalStrengthPixmaps[index]);
           offset = m_signalStrengthPixmaps[0].width();
        }

        if (networkType == "WLAN")
            p->drawPixmap(offset, 0, m_networkTypePixmaps[Wifi]);
        else if (networkType == "wimax")
            p->drawPixmap(offset, 0, m_networkTypePixmaps[Wimax]);
        else if (networkType == "GPRS")
            p->drawPixmap(offset, 0, m_networkTypePixmaps[GPRS]);
        else if (networkType == "bluetooth")
            p->drawPixmap(offset, 0, m_networkTypePixmaps[Bluetooth]);
        else
            p->drawPixmap(offset, 0, m_networkTypePixmaps[Ethernet]);


        setImplicitWidth(offset + m_networkTypePixmaps[0].width());
        setImplicitHeight(m_networkTypePixmaps[0].height());
    }
    else
    {
        p->drawPixmap(0, 0, m_signalStrengthPixmaps[0]);
        setImplicitWidth(m_signalStrengthPixmaps[0].width());
        setImplicitHeight(m_signalStrengthPixmaps[0].height());
    }
}

void NetworkIndicator::themeUpdated()
{
    QString path = QString("/usr/share/themes/%1/icons/statusbar/").arg(m_themeItem->value().toString());
    if (QDir(path).exists())
    {
        m_networkTypePixmaps[GPRS].load(path + "3g-on.png");
        m_networkTypePixmaps[Wifi].load(path + "wifi-on.png");
        m_networkTypePixmaps[Bluetooth].load(path + "bluetooth-on.png");
        m_networkTypePixmaps[Wimax].load(path + "4g-on.png");
        m_networkTypePixmaps[Ethernet].load(path + "network-ethernet.png");

        m_signalStrengthPixmaps[0].load(path + "signal-none.png");
        m_signalStrengthPixmaps[1].load(path + "signal-weak.png");
        m_signalStrengthPixmaps[2].load(path + "signal-ok.png");
        m_signalStrengthPixmaps[3].load(path + "signal-good.png");
        m_signalStrengthPixmaps[4].load(path + "signal-strong.png");
    }
}

QML_DECLARE_TYPE(NetworkIndicator);

