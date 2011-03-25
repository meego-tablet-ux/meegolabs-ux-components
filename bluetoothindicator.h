#ifndef BLUETOOTHINDICATOR_H
#define BLUETOOTHINDICATOR_H

#include <QObject>
#include <QDeclarativeItem>
#include <QPixmap>

class ContextProperty;
class MGConfItem;

class BluetoothIndicator : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(bool active READ getActive WRITE setActive)
    Q_PROPERTY(bool hideOnActiveNetwork READ getHideOnActiveNetwork WRITE setHideOnActiveNetwork)
public:
    explicit BluetoothIndicator(QDeclarativeItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool getHideOnActiveNetwork() {
        return m_hideOnActiveNetwork;
    }
    void setHideOnActiveNetwork(bool hide) {
        m_hideOnActiveNetwork = hide;
    }

    bool getActive() {
        return m_active;
    }
    void setActive(bool active);

private slots:
    void updateIcon();
    void themeUpdated();
    void networkTypeUpdated();

private:
    ContextProperty *m_bluetoothEnabledProperty;
    ContextProperty *m_networkTypeProperty;
    MGConfItem *m_themeItem;
    QPixmap m_pixmap;
    bool m_activeBluetoothNetwork;
    bool m_hideOnActiveNetwork;
    bool m_active;
};

#endif // BLUETOOTHINDICATOR_H
