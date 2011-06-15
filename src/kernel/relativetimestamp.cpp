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
            //: Fuzzy date description
            return tr("Just now");
        } else if (mDatetime.addSecs(2 * 60) > now) {
            //: Fuzzy date description
            return tr("1 min ago");
        } else if (mDatetime.addSecs(29 * 60) > now) {
            //: Fuzzy date description - %n is a number
            return tr("%n min(s) ago", "", int(mDatetime.secsTo(now)/60));
        } else if (mDatetime.addSecs(59 * 60) > now) {
            //: Fuzzy date description
            return tr("Half an hour ago");
        } else if (mDatetime.addSecs(119 * 60) > now) {
            //: Fuzzy date description
            return tr("An hour ago");
        } else if (mDatetime.addSecs(239 * 60) > now) {
            //: Fuzzy date description
            return tr("A couple of hours ago");
        } else {
            //: Fuzzy date description - %n is a number
            return tr("%n hours ago", "", int(mDatetime.secsTo(now)/(60 * 60)));
        }
    } else {
        //: QDateTime format string: M is numeric month, d is num. day, yy is year; e.g. 1/31/11
        //: translator: reorder / reformat, but make sure to use these ASCII M, d, and yy format codes
        QString dateFormat = tr("M/d/yy");
        QString formattedDate = dtDate.toString(dateFormat);

        //: %1 is formatted date, %2 is fuzzy date description, e.g. 1/31/11 - Last week
        QString fuzzyDateFormat = tr("%1 - %2");

        if (dtDate.addDays(1) == nowDate) {
            //: Fuzzy date description
            return tr("Yesterday");
        } else if (dtDate.addDays(7) >= nowDate) {
            return fuzzyDateFormat.arg(formattedDate, dtDate.toString("dddd"));
        } else if (dtDate.addDays(14) >= nowDate) {
            //: Fuzzy date description
            return fuzzyDateFormat.arg(formattedDate, tr("Last week"));
        } else if (dtDate.addDays(21) >= nowDate) {
            //: Fuzzy date description
            return fuzzyDateFormat.arg(formattedDate, tr("A couple of weeks ago"));
        } else if (dtDate.month() == nowMonth) {
            int weeks = dtDate.daysTo(nowDate) / 7;
            //: Fuzzy date description - %n is a number
            return fuzzyDateFormat.arg(formattedDate, tr("%n week(s) ago", "", weeks));
        } else if (dtDate.addMonths(1).month() == nowMonth) {
            //: Fuzzy date description
            return fuzzyDateFormat.arg(formattedDate, tr("Last month"));
        } else if (dtDate.addMonths(3) >= nowDate) {
            //: Fuzzy date description
            return fuzzyDateFormat.arg(formattedDate, tr("A couple of months ago"));
        } else if (dtDate.addMonths(12) > nowDate) {
            int months = nowMonth - dtDate.month();
            //If this is true, we're wrapping around a year, add 12
            if (months < 0)
                months += 12;
            //: Fuzzy date description - %n is a number
            return fuzzyDateFormat.arg(formattedDate, tr("%n month(s) ago","", months));
        } else if (dtDate.addMonths(23) >= nowDate) {
            //: Fuzzy date description
            return fuzzyDateFormat.arg(formattedDate, tr("Last year"));
        } else {
            int years = nowDate.year() - dtDate.year();
            //: Fuzzy date description - %n is a number
            return fuzzyDateFormat.arg(formattedDate, tr("%n year(s) ago", "", years));
        }
    }
}

