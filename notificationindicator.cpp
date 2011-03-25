#include <QFile>
#include <QPainter>
#include <QMap>
#include <QTimer>
#include <MGConfItem>
#include <mremoteaction.h>

#include "notificationindicator.h"
#include <contextsubscriber/contextproperty.h>

NotificationIndicator::NotificationIndicator(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    m_active(false)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);

    m_themeItem = new MGConfItem("/meego/ux/theme");
    connect(m_themeItem, SIGNAL(valueChanged()), this, SLOT(themeUpdated()));

    themeUpdated();

    setActive(true);
}

NotificationIndicator::~NotificationIndicator()
{
}

void NotificationIndicator::setActive(bool active)
{
    if (active == m_active)
        return;

    m_active = active;
    if (active)
    {
        m_lastProperty = new ContextProperty("Notifications.Last", this);
        connect(m_lastProperty, SIGNAL(valueChanged()), this, SLOT(notifySink()));

        m_unreadProperty = new ContextProperty("Notifications.Unread", this);
        connect(m_unreadProperty, SIGNAL(valueChanged()), this, SLOT(updateIcon()));
    }
    else
    {
        if (m_lastProperty)
        {
            disconnect(m_lastProperty);
            delete m_lastProperty;
            m_lastProperty = 0;
        }

        if (m_unreadProperty)
        {
            disconnect(m_unreadProperty);
            delete m_unreadProperty;
            m_unreadProperty = 0;
        }
    }
}

void NotificationIndicator::updateIcon()
{
    QGraphicsItem::update(boundingRect());
}

void NotificationIndicator::notifySink()
{
    QMap<QString, QVariant> map = m_lastProperty->value().toMap();
    if (!map.isEmpty())
    {
        emit notify(
                    map["notificationType"].toString(),
                    map["notificationSummary"].toString(),
                    map["notificationBody"].toString(),
                    map["notificationAction"].toString(),
                    map["notificationImage"].toString());
    }
}

void NotificationIndicator::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!m_active)
        return;

    if (m_unreadProperty->value().toBool())
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

void NotificationIndicator::themeUpdated()
{
    QString path = QString("/usr/share/themes/%1/icons/statusbar/notification-on.png").arg(m_themeItem->value().toString());
    if (QFile::exists(path))
    {
        m_pixmap.load(path);
    }
}

void NotificationIndicator::triggerAction(QString action)
{
    if (!action.isEmpty())
    {
        MRemoteAction *remote = new MRemoteAction(action);
        remote->trigger();
        delete remote;
    }
}

QML_DECLARE_TYPE(NotificationIndicator);

