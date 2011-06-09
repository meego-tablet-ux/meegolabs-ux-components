/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QDebug>

#include <QDate>
#include <QDateTime>
#include <QTime>
#include <qdeclarative.h>

#include "localehelper.h"

LocaleHelper::LocaleHelper(QObject *parent): QObject(parent)
{
    m_24hour = false;

    //: translator: set to 1 for default 24-hour time format (0 for 12-hour)
    QString time24 = tr("0", "timeformat");
    QChar ch = time24.at(0);
    if (ch == '1')
        m_24hour = true;
}

QString LocaleHelper::localDate(const QDate &date, int format) const
{
    if (format <= DateBEGIN || format >= DateEND) {
        qWarning() << "LocaleHelper: invalid date format: " << format;
        return QString();
    }

    return date.toString(formatString(format));
}

QString LocaleHelper::localTime(const QTime &time, int format) const
{
    if (format <= TimeBEGIN || format >= TimeEND) {
        qWarning() << "LocaleHelper: invalid time format: " << format;
        return QString();
    }

    int hour = time.hour();
    int minute = time.minute();

    //: string to represent AM time when 12-hour time format is displayed
    QString am = tr("AM");

    //: string to represent PM time when 12-hour time format is displayed
    QString pm = tr("PM");

    QString ampm = (hour < 12) ? am : pm;
    int hour12 = (hour % 12) ? (hour % 12) : 12;

    switch (format) {
    case TimeFull:
        if (m_24hour)
            return formatString(TimeFull24);
        else return formatString(TimeFull12);

    case TimeFull12:
        //: AM time format string (translator: update order / format)
        //: %1 = hours (no leading zero), %2 = minutes (leading zero if needed), %3 = AM/PM string
        return tr("%1:%2 %3").arg(hour12).arg(minute, 2, QChar('0')).arg(ampm);

    case TimeFull24:
        //: 24-hour time format string (translator: update order / format)
        //: %1 = hours (no leading zero), %2 = minutes (leading zero if needed)
        return tr("%1:%2").arg(hour).arg(minute, 2, QChar('0'));

    case TimeQtLong:
        return time.toString(m_locale.timeFormat(QLocale::LongFormat));

    case TimeQtShort:
        return time.toString(m_locale.timeFormat(QLocale::ShortFormat));

    default:
        qWarning() << "LocaleHelper: unhandled time format: " << format;
        return QString();
    }
}

QString LocaleHelper::localDateTime(const QDateTime &datetime, int format) const
{
    if (format <= DateTimeBEGIN || format >= DateTimeEND) {
        qWarning() << "LocaleHelper: invalid datetime format: " << format;
        return QString();
    }

    return datetime.toString(formatString(format));
}

QString LocaleHelper::currentDate(int format) const
{
    return localDate(QDate::currentDate(), format);
}

QString LocaleHelper::currentTime(int format) const
{
    return localTime(QTime::currentTime(), format);
}

QString LocaleHelper::currentDateTime(int format) const
{
    return localDateTime(QDateTime::currentDateTime(), format);
}

QString LocaleHelper::numericDateOrder() const
{
    //: translator: this order will determine order of date picker widgets
    //: default order for numeric date (m = month, d = date, y = year)
    //: the string should contain these exact three characters in some order
    QString str = tr("mdy", "dateorder");

    if (str.length() == 3 && str.indexOf("m") != -1 &&
        str.indexOf("d") != -1 &&  str.indexOf("y") != -1) {
        return str;
    }
    return QString();
}

int LocaleHelper::firstDayOfWeek() const
{
    //: translator: identify the first day of the week in target culture
    //: 1 = Sunday, 2 = Monday, etc.
    QString day = tr("1", "firstday");
    QChar ch = day.at(0);
    int dayOfWeek = DaySunday;
    if (ch >= '1' && ch <= '7')
        dayOfWeek = ch.toAscii() - '0';
    return dayOfWeek;
}

QString LocaleHelper::decimalPoint() const
{
    return m_locale.decimalPoint();
}

//
// protected member functions
//

QString LocaleHelper::formatString(int format) const
{
    switch (format) {
    case DateFullLong:
        //: QDateTime format string (translator: update order / format) - See http://doc.qt.nokia.com/4.7/qdatetime.html#toString
        //: dddd = full day of week, MMMM = full month, d = day of month, yyyy = year (e.g. Monday, January 31, 2011)
        return tr("dddd, MMMM d, yyyy");

    case DateFull:
        //: QDateTime format string (translator: update order / format)
        //: MMMM = full month, d = day of month, yyyy = full year (e.g. January 31, 2011)
        return tr("MMMM d, yyyy");

    case DateFullShort:
        //: QDateTime format string (translator: update order / format)
        //: MMM = short month, d = day of month, yyyy = full year (e.g. Jan 31 2011)
        return tr("MMM d yyyy");

    case DateFullNum:
        //: QDateTime format string (translator: update order / format)
        //: M = numeric month, d = day of month, yyyy = full year (e.g. 1/31/2011)
        return tr("M/d/yyyy");

     case DateFullNumShort:
        //: QDateTime format string (translator: update order / format)
        //: M = numeric month, d = day of month, yy = year (e.g. 1/31/11)
        return tr("M/d/yy");

    case DateWeekdayMonthDay:
        //: QDateTime format string (translator: update order / format)
        //: dddd = full day of week, MMMM = full month, d = day of month (e.g. Monday, January 31)
        return tr("dddd, MMMM d");

    case DateWeekdayDayShort:
        //: QDateTime format string (translator: update order / format)
        //: ddd = short day of week, d = day of month (e.g. Mon 31)
        return tr("ddd d");

    case DateMonthDay:
        //: QDateTime format string (translator: update order / format)
        //: MMMM = full month, d = day of month (e.g. January 31)
        return tr("MMMM d");

    case DateMonthYear:
        //: QDateTime format string (translator: update order / format)
        //: MMMM = full month, yyyy = full year (e.g. January 2011)
        return tr("MMMM yyyy");

    case DateMonthYearShort:
        //: QDateTime format string (translator: update order / format)
        //: MMM = short month, yyyy = full year (e.g. Jan 2011)
        return tr("MMM yyyy");

    case DateDay:
        //: QDateTime format string (translator: you probably won't change this)
        //: d = day of month (e.g. 31, no leading zero)
        return tr("d");

    case DateWeekday:
        //: QDateTime format string (translator: you probably won't change this)
        //: dddd = full day of week (e.g. Monday)
        return tr("dddd");

    case DateWeekdayShort:
        //: QDateTime format string (translator: you probably won't change this)
        //: ddd = short day of week (e.g. Mon)
        return tr("ddd");

    case DateMonth:
        //: QDateTime format string (translator: you probably won't change this)
        //: MMMM = full month (e.g. January)
        return tr("MMMM");

    case DateMonthShort:
        //: QDateTime format string (translator: you probably won't change this)
        //: MMM = short month (e.g. Jan)
        return tr("MMM");

    case DateYear:
        //: QDateTime format string (translator: you probably won't change this)
        //: yyyy (e.g. 2011)
        return tr("yyyy");

    case DateQtLong:
        // e.g. Monday, January 31, 2011
        return m_locale.dateFormat(QLocale::LongFormat);

    case DateQtShort:
        // e.g. 1/31/11
        return m_locale.dateFormat(QLocale::ShortFormat);

    case DateTimeQtLong:
        return m_locale.dateTimeFormat(QLocale::LongFormat);

    case DateTimeQtShort:
        return m_locale.dateTimeFormat(QLocale::ShortFormat);

    default:
        qWarning() << "LocaleHelper: unexpected format string";
        return QString();
    }
}

QML_DECLARE_TYPE(LocaleHelper);
