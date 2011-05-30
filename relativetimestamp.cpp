#include "relativetimestamp.h"
#include "timestampupdater.h"

#include <QDate>

RelativeTimeStamp::RelativeTimeStamp(QObject *parent) :
    QObject(parent)
{
    TimeStampUpdater::get().append(this);
}

RelativeTimeStamp::~RelativeTimeStamp()
{
    TimeStampUpdater::get().remove(this);
}

QDateTime RelativeTimeStamp::getDateTime() const
{
    return mDatetime;
}

void RelativeTimeStamp::setDateTime(QDateTime &datetime)
{
    mDatetime = datetime;
    emit textChanged();
}

const QString RelativeTimeStamp::text() const
{
    QDateTime now = QDateTime::currentDateTime();
    QDate nowDate = now.date();
    QDate dtDate = mDatetime.date();
    int nowMonth = nowDate.month();

    if (mDatetime == QDateTime())
        return "";

    if (nowDate == dtDate) {
        if (mDatetime.addSecs(60) > now) {
            return tr("Just Now");
        } else if (mDatetime.addSecs(2 * 60) > now) {
            return tr("1 min ago");
        } else if (mDatetime.addSecs(29 * 60) > now) {
            return tr("%1 mins ago", "1 is number of minutes").arg(
                    QString::number(int(mDatetime.secsTo(now)/60)));
        } else if (mDatetime.addSecs(59 * 60) > now) {
            return tr("Half an hour ago");
        } else if (mDatetime.addSecs(119 * 60) > now) {
            return tr("An hour ago");
        } else if (mDatetime.addSecs(239 * 60) > now) {
            return tr("A couple of hours ago");
        } else {
            return tr("%1 hours ago", "1 is number of hours").arg(
                    QString::number(int(mDatetime.secsTo(now)/(60 * 60))));
        }
    } else {
        if (dtDate.addDays(1) == nowDate) {
            return tr("Yesterday");
        } else if (dtDate.addDays(7) >= nowDate) {
            return tr("%1/%2/%3 - %4", "1 is month, 2 is day, 3 is year, 4 is day name").arg(
                    dtDate.toString("M"),
                    dtDate.toString("d"),
                    dtDate.toString("yy"),
                    dtDate.toString("dddd"));
        } else if (dtDate.addDays(14) >= nowDate) {
            return tr("%1/%2/%3 - Last week", "1 is month, 2 is day, 3 is year").arg(
                    dtDate.toString("M"),
                    dtDate.toString("d"),
                    dtDate.toString("yy"));
        } else if (dtDate.addDays(21) >= nowDate) {
            return tr("%1/%2/%3 - A couple of weeks ago", "1 is month, 2 is day, 3 is year").arg(
                    dtDate.toString("M"),
                    dtDate.toString("d"),
                    dtDate.toString("yy"));
        } else if (dtDate.month() == nowMonth) {
            return tr("%1/%2/%3 - %4 weeks ago", "1 is month, 2 is day, 3 is year, 4 is number of weeks").arg(
                    dtDate.toString("M"),
                    dtDate.toString("d"),
                    dtDate.toString("yy"),
                    QString::number(int(dtDate.daysTo(nowDate)/7)));
        } else if (dtDate.addMonths(1).month() == nowMonth) {
            return tr("%1/%2/%3 - Last month", "1 is month, 2 is day, 3 is year").arg(
                    dtDate.toString("M"),
                    dtDate.toString("d"),
                    dtDate.toString("yy"));
        } else if (dtDate.addMonths(3) >= nowDate) {
            return tr("%1/%2/%3 - A couple of months ago", "1 is month, 2 is day, 3 is year").arg(
                    dtDate.toString("M"),
                    dtDate.toString("d"),
                    dtDate.toString("yy"));
        } else if (dtDate.addMonths(12) > nowDate) {
            int nowMonth2 = nowMonth;
            //If this is true, we're wrapping around a year, add 12 for the month math
            if (nowMonth2 < dtDate.month())
                nowMonth2 += 12;
            return tr("%1/%2/%3 - %4 months ago", "1 is month, 2 is day, 3 is year").arg(
                    dtDate.toString("M"),
                    dtDate.toString("d"),
                    dtDate.toString("yy"),
                    QString::number(nowMonth2-dtDate.month()));
        } else if (dtDate.addMonths(23) >= nowDate) {
            return tr("%1/%2/%3 - Last year", "1 is month, 2 is day, 3 is year").arg(
                    dtDate.toString("M"),
                    dtDate.toString("d"),
                    dtDate.toString("yy"));
        } else {
            return tr("%1/%2/%3 - %4 years ago", "1 is month, 2 is day, 3 is year").arg(
                    dtDate.toString("M"),
                    dtDate.toString("d"),
                    dtDate.toString("yy"),
                    QString::number(nowDate.year() - dtDate.year()));
        }
    }
}

