#ifndef BATTERYINDICATOR_H
#define BATTERYINDICATOR_H

#include <QObject>
#include <QDeclarativeItem>
#include <QDir>
#include <QPixmap>

class ContextProperty;
class MGConfItem;

class BatteryIndicator : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(bool active READ getActive WRITE setActive)
public:
    explicit BatteryIndicator(QDeclarativeItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setActive(bool active);
    bool getActive() {
        return m_active;
    }

private slots:
    void updateIcon();
    void themeUpdated();

private:
    ContextProperty *m_chargePercentageProperty;
    ContextProperty *m_onBatteryProperty;
    ContextProperty *m_lowBatteryProperty;
    ContextProperty *m_isChargingProperty;

    MGConfItem *m_themeItem;
    QPixmap m_pixmaps[6];
    bool m_active;
    int m_currentPixmapIndex;
    bool m_charging;
};

#endif // BATTERYINDICATOR_H
