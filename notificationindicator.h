#ifndef NOTIFICATIONINDICATOR_H
#define NOTIFICATIONINDICATOR_H

#include <QObject>
#include <QDeclarativeItem>
#include <QPixmap>

class ContextProperty;
class MGConfItem;

class NotificationIndicator : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(bool active READ getActive WRITE setActive)
public:
    explicit NotificationIndicator(QDeclarativeItem *parent = 0);
    ~NotificationIndicator();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool getActive() {
        return m_active;
    }
    void setActive(bool active);

signals:
    void notify(QString type, QString summary, QString body, QString action, QString image);

public slots:
    void triggerAction(QString action);

private slots:
    void updateIcon();
    void themeUpdated();
    void notifySink();

private:
    ContextProperty *m_lastProperty;
    ContextProperty *m_unreadProperty;
    MGConfItem *m_themeItem;
    QPixmap m_pixmap;
    bool m_active;
};

#endif // NOTIFICATIONINDICATOR_H
