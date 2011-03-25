#include <QColor>
#include <QFile>
#include <QPainter>
#include <MGConfItem>

#include "bluetoothindicator.h"
#include <contextsubscriber/contextproperty.h>

BluetoothIndicator::BluetoothIndicator(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    m_activeBluetoothNetwork(false),
    m_hideOnActiveNetwork(true),
    m_active(false)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);

    m_themeItem = new MGConfItem("/meego/ux/theme");
    connect(m_themeItem, SIGNAL(valueChanged()), this, SLOT(themeUpdated()));

    themeUpdated();

    setActive(true);
}

void BluetoothIndicator::setActive(bool active)
{
    if (active == m_active)
        return;

    m_active = active;
    if (active)
    {
        m_bluetoothEnabledProperty = new ContextProperty("Bluetooth.Enabled", this);
        connect(m_bluetoothEnabledProperty, SIGNAL(valueChanged()), this, SLOT(updateIcon()));

        m_networkTypeProperty = new ContextProperty("Internet.NetworkType", this);
        connect(m_networkTypeProperty, SIGNAL(valueChanged()), this, SLOT(networkTypeUpdated()));
    }
    else
    {
        if (m_bluetoothEnabledProperty)
        {
            disconnect(m_bluetoothEnabledProperty);
            delete m_bluetoothEnabledProperty;
            m_bluetoothEnabledProperty = 0;
        }

        if (m_networkTypeProperty)
        {
            disconnect(m_networkTypeProperty);
            delete m_networkTypeProperty;
            m_networkTypeProperty = 0;
        }
    }
}

void BluetoothIndicator::updateIcon()
{
    QGraphicsItem::update(boundingRect());
}

void BluetoothIndicator::networkTypeUpdated()
{
    if (!m_hideOnActiveNetwork)
        return;

    if (m_networkTypeProperty->value().toString() == "bluetooth")
    {
        m_activeBluetoothNetwork = true;
        QGraphicsItem::update(boundingRect());
    }
    else
    {
        bool previousState = m_activeBluetoothNetwork;
        m_activeBluetoothNetwork = false;
        if (previousState)
            QGraphicsItem::update(boundingRect());
    }
}

void BluetoothIndicator::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!m_active)
        return;

    if (m_bluetoothEnabledProperty->value().toBool() &&
            (!m_hideOnActiveNetwork || !m_activeBluetoothNetwork))
    {
        p->drawPixmap(0, 0, m_pixmap);
        setImplicitWidth(m_pixmap.width());
        setImplicitHeight(m_pixmap.height());
    }
    else
    {
        setImplicitWidth(0);
        setImplicitHeight(0);
    }
}

void BluetoothIndicator::themeUpdated()
{
    QString path = QString("/usr/share/themes/%1/icons/statusbar/bluetooth-on.png").arg(m_themeItem->value().toString());
    if (QFile::exists(path))
    {
        m_pixmap.load(path);
    }
}

QML_DECLARE_TYPE(BluetoothIndicator);

