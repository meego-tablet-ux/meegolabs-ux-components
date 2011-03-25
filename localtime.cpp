#include <qdeclarative.h>
#include "localtime.h"

LocalTime::LocalTime(QObject *parent) : QObject(parent)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(forwardTimeout()));
}

void LocalTime::setInterval(int interval)
{
    m_timer.stop();
    m_timer.setInterval(interval);
    if (interval > 0)
    {
        m_timer.start();
        forwardTimeout();
    }
}

QML_DECLARE_TYPE(LocalTime);
