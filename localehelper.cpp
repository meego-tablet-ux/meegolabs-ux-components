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

    return time.toString(formatString(format));
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
    //: default order for numeric date (m = month, d = date, y = year)
    //: this string should contain these exact three characters in some order
    QString str = tr("mdy");

    if (str.length() == 3 && str.indexOf("m") != -1 &&
        str.indexOf("d") != -1 &&  str.indexOf("y") != -1) {
        return str;
    }
    return QString();
}

int LocaleHelper::firstDayOfWeek() const
{
    //: localize this first day of the week (1 = Sunday, 2 = Monday, ...)
    QString day = tr("1");
    QChar ch = day.at(0);
    int dayOfWeek = DaySunday;
    if (ch >= '1' && ch <= '7')
        dayOfWeek = ch.toAscii() - '0';
    return dayOfWeek;
}

QString LocaleHelper::decimalPoint() const
{
    return QString(m_locale.decimalPoint());
}

//
// protected member functions
//

QString LocaleHelper::formatString(int format) const
{
    switch (format) {
    case DateFullLong:
        // e.g. Monday, January 31, 2011
        return m_locale.dateFormat(QLocale::LongFormat);

    case DateFull:
        //: MMMM = full month, d = day of month, yyyy = full year (e.g. January 31, 2011)
        return tr("MMMM d, yyyy");

    case DateFullShort:
        //: MMM = short month, d = day of month, yyyy = full year (e.g. Jan 31 2011)
        return tr("MMM d yyyy");

    case DateFullNumShort:
        // e.g. 1/31/11
        return m_locale.dateFormat(QLocale::ShortFormat);

    case DateWeekdayMonthDay:
        //: dddd = full day of week, MMMM = full month, d = day of month (e.g. Monday, January 31)
        return tr("dddd, MMMM d");

    case DateWeekdayDayShort:
        //: ddd = day of week, d = day of month (e.g. Mon 31)
        return tr("ddd d");

    case DateMonthYear:
        //: MMMM = full month, yyyy = full year (e.g. January 2011)
        return tr("MMMM yyyy");

    case DateMonthYearShort:
        //: MMM = short month, yyyy = full year (e.g. Jan 2011)
        return tr("MMM yyyy");

    case DateDay:
        //: d = day of month (e.g. 31, no leading zero)
        return tr("d");

    case DateWeekday:
        //: dddd = day of week (e.g. Monday)
        return tr("dddd");

    case DateWeekdayShort:
        //: ddd = day of week (e.g. Mon)
        return tr("ddd");

    case DateMonth:
        //: MMMM = full month (e.g. January)
        return tr("MMMM");

    case DateMonthShort:
        //: MMM = short month (e.g. Jan)
        return tr("MMM");

    case DateYear:
        //: yyyy (e.g. 2011)
        return tr("yyyy");

    case TimeFullLong:
        return m_locale.timeFormat(QLocale::LongFormat);

    case TimeFullShort:
        return m_locale.timeFormat(QLocale::ShortFormat);

    case DateTimeFullLong:
        return m_locale.dateTimeFormat(QLocale::LongFormat);

    case DateTimeFullShort:
        return m_locale.dateTimeFormat(QLocale::ShortFormat);

    default:
        qWarning() << "LocaleHelper: unexpected format string";
        return QString();
    }
}

QML_DECLARE_TYPE(LocaleHelper);
