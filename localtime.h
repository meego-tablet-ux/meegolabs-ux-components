#ifndef LOCALTIME_H
#define LOCALTIME_H

#include <QObject>
#include <QDateTime>
#include <QLocale>
#include <QTimer>

class LocalTime : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString shortTime READ getShortTime NOTIFY timeout)
    Q_PROPERTY(QString longTime READ getLongTime NOTIFY timeout)
    Q_PROPERTY(QString shortDate READ getShortDate)
    Q_PROPERTY(QString longDate READ getLongDate)
    Q_PROPERTY(int interval READ getInterval WRITE setInterval);

public:
    explicit LocalTime(QObject *parent = 0);

    QString getShortTime() const {
        return QDateTime::currentDateTime().time().currentTime()
                .toString(QLocale::system().timeFormat(QLocale::ShortFormat));
    }
    QString getLongTime() const {
        return QDateTime::currentDateTime().time().currentTime()
                .toString(QLocale::system().timeFormat(QLocale::LongFormat));
    }
    QString getShortDate() const {
        return QDateTime::currentDateTime().date()
                .toString(QLocale::system().dateFormat(QLocale::ShortFormat));
    }
    QString getLongDate() const {
        return QDateTime::currentDateTime().date()
                .toString(QLocale::system().dateFormat(QLocale::LongFormat));
    }
    int getInterval() {
        return m_timer.interval();
    }
    void setInterval(int interval);

signals:
    void timeout();

private slots:
    void forwardTimeout() {
        emit timeout();
    }

private:
    QTimer m_timer;
};

#endif // LOCALTIME_H
