#ifndef NETWORKINDICATOR_H
#define NETWORKINDICATOR_H

#include <QObject>
#include <QDeclarativeItem>
#include <QDir>
#include <QPixmap>

class ContextProperty;
class MGConfItem;

class NetworkIndicator : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(bool active READ getActive WRITE setActive)
public:
    explicit NetworkIndicator(QDeclarativeItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    enum NetworkType {
        Wifi = 0,
        Wimax = 1,
        GPRS = 2,
        Bluetooth = 3,
        Ethernet = 4
    };

    void setActive(bool active);
    bool getActive() {
        return m_active;
    }

private slots:
    void updateIcon();
    void themeUpdated();

private:
    ContextProperty *m_networkTypeProperty;
    ContextProperty *m_networkStateProperty;
    ContextProperty *m_signalStrengthProperty;
    bool m_active;

    MGConfItem *m_themeItem;
    QPixmap m_networkTypePixmaps[5];
    QPixmap m_signalStrengthPixmaps[5];
};

#endif // NETWORKINDICATOR_H
