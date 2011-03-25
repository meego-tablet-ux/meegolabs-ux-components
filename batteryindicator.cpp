#include <QColor>
#include <QFile>
#include <QPainter>
#include <MGConfItem>

#include "batteryindicator.h"
#include <contextsubscriber/contextproperty.h>

BatteryIndicator::BatteryIndicator(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    m_active(false),
    m_currentPixmapIndex(4),
    m_charging(false)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);

    m_themeItem = new MGConfItem("/meego/ux/theme");
    connect(m_themeItem, SIGNAL(valueChanged()), this, SLOT(themeUpdated()));

    themeUpdated();

    setActive(true);
}

void BatteryIndicator::setActive(bool active)
{
    if (m_active == active)
        return;

    m_active = active;
    if (m_active)
    {
        m_chargePercentageProperty = new ContextProperty("Battery.ChargePercentage", this);
        connect(m_chargePercentageProperty, SIGNAL(valueChanged()), this, SLOT(updateIcon()));

        m_onBatteryProperty = new ContextProperty("Battery.OnBattery", this);
        connect(m_onBatteryProperty, SIGNAL(valueChanged()), this, SLOT(updateIcon()));

        m_lowBatteryProperty = new ContextProperty("Battery.LowBattery", this);
        connect(m_lowBatteryProperty, SIGNAL(valueChanged()), this, SLOT(updateIcon()));

        m_isChargingProperty = new ContextProperty("Battery.IsCharging", this);
        connect(m_isChargingProperty, SIGNAL(valueChanged()), this, SLOT(updateIcon()));

        updateIcon();
    }
    else
    {
        if (m_chargePercentageProperty)
        {
            disconnect(m_chargePercentageProperty);
            delete m_chargePercentageProperty;
            m_chargePercentageProperty = 0;
        }

        if (m_onBatteryProperty)
        {
            disconnect(m_onBatteryProperty);
            delete m_onBatteryProperty;
            m_onBatteryProperty = 0;
        }

        if (m_lowBatteryProperty)
        {
            disconnect(m_lowBatteryProperty);
            delete m_lowBatteryProperty;
            m_lowBatteryProperty = 0;
        }

        if (m_isChargingProperty)
        {
            disconnect(m_isChargingProperty);
            delete m_isChargingProperty;
            m_isChargingProperty = 0;
        }
    }
}

void BatteryIndicator::updateIcon()
{
    if (m_lowBatteryProperty->value().toBool())
    {
        m_currentPixmapIndex = 4;
    }
    else
    {
        double percent = m_chargePercentageProperty->value().toDouble();
        if (percent > 75.0)
            m_currentPixmapIndex = 0;
        else if (percent > 50.0)
            m_currentPixmapIndex = 1;
        else if (percent > 25.0)
            m_currentPixmapIndex = 2;
        else
            m_currentPixmapIndex = 3;
    }

    m_charging = m_isChargingProperty->value().toBool();

    QGraphicsItem::update(boundingRect());
}

void BatteryIndicator::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
    p->drawPixmap(0, 0, m_pixmaps[m_currentPixmapIndex]);
    if (m_charging)
    {
        p->drawPixmap(0, 0, m_pixmaps[5]);
    }
}

void BatteryIndicator::themeUpdated()
{
    QString path = QString("/usr/share/themes/%1/icons/statusbar/").arg(m_themeItem->value().toString());
    if (QDir(path).exists())
    {
        m_pixmaps[0].load(path + "battery-charged.png");
        m_pixmaps[1].load(path + "battery-good.png");
        m_pixmaps[2].load(path + "battery-fine.png");
        m_pixmaps[3].load(path + "battery-caution.png");
        m_pixmaps[4].load(path + "battery-low.png");
        m_pixmaps[5].load(path + "battery-charging-overlay.png");

        setImplicitWidth(m_pixmaps[0].width());
        setImplicitHeight(m_pixmaps[0].height());
    }
}

QML_DECLARE_TYPE(BatteryIndicator);

