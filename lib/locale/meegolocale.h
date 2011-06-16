/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGO_LOCALE_H
#define MEEGO_LOCALE_H

#include "meegolocale_global.h"
#include <QObject>
#include <QLocale>
#include <QList>

namespace meego {

// This is a class for providing locale-related information

class LocalePrivate;

class LOCALESHARED_EXPORT Locale : public QObject
{
    Q_OBJECT;

public:
    explicit Locale (QObject *parent = 0);

    enum DateTimeFormat {
        DateBEGIN = 100,
        DateFullLong,         // Monday, January 31, 2011
        DateFull,             // January 31, 2011
        DateFullShort,        // Jan 31 2011
        DateFullNum,          // 1/31/2011
        DateFullNumShort,     // 1/31/11
        DateWeekdayMonthDay,  // Monday, January 31
        DateWeekdayDayShort,  // Mon 31
        DateMonthDay,         // January 31
        DateMonthYear,        // January 2011
        DateMonthYearShort,   // Jan 2011
        DateDay,              // 31 (no leading zero)
        DateWeekday,          // Monday
        DateWeekdayShort,     // Mon
        DateMonth,            // January
        DateMonthShort,       // Jan
        DateYear,             // 2011

        // NOT YET SUPPORTED - Ask Geoff
        DateWeekYear,       // January 30 - February 5, 2011
        DateWeekYearShort,  // Jan 30 - Feb 5, 2011

        // pass through to Qt date formats
        DateQtLong,   // Monday, January 31, 2011
        DateQtShort,  // 1/31/11
        DateEND,

        TimeBEGIN = 200,
        TimeFull,    // 9:02 PM or 21:02
        TimeFull12,  // 9:02 PM
        TimeFull24,  // 21:02

        // pass through to Qt date formats
        TimeQtLong,   // 9:02:03 PM TZ
        TimeQtShort,  // 9:02 PM (or 24-hr time if set for locale)
        TimeEND,

        DateTimeBEGIN = 300,
        // pass through to Qt date formats
        DateTimeQtLong,   // Monday, January 31, 2011 1:02:03 AM TZ
        DateTimeQtShort,  // 1/31/11 1:02 AM
        DateTimeEND
    };
    Q_ENUMS(DateTimeFormat);

    enum DayOfWeek {
        DaySunday = 1,
        DayMonday,
        DayTuesday,
        DayWednesday,
        DayThursday,
        DayFriday,
        DaySaturday
    };
    Q_ENUMS(DayOfWeek);

    enum DateOrder {
        YMD,
        DMY,
        MDY
    };
    Q_ENUMS( DateOrder );

    enum HourFormat {
        defaultHrs,
        Hrs12,
        Hrs24
    };
    Q_ENUMS( HourFormat );

    // date/time formatting
    Q_INVOKABLE QString localDate(const QDate &date, int format) const;
    Q_INVOKABLE QString localTime(const QTime &time, int format) const;
    Q_INVOKABLE QString localDateTime(const QDateTime &datetime, int format) const;

    Q_INVOKABLE QString currentDate(int format) const;
    Q_INVOKABLE QString currentTime(int format) const;
    Q_INVOKABLE QString currentDateTime(int format) const;

    // returns a three-character string with the letters 'd', 'm', and 'y' in
    //   the order that those should appear for this locale, e.g. "mdy" for
    //   American 1/31/2011, "dmy" for European 31/1/2011
    Q_INVOKABLE QString numericDateOrder() const;

    // localized first day of the week (returns DayOfWeek enum)
    Q_INVOKABLE int firstDayOfWeek() const;
    Q_INVOKABLE void setFirstDayOfWeek( DayOfWeek dayofWeek );
    Q_INVOKABLE int getDefaultFirstDayOfWeek() const;

    // localized decimal point
    Q_INVOKABLE QString decimalPoint() const;

    Q_INVOKABLE QString getCurrentLanguageCode() const;
    Q_INVOKABLE void setLanguageCode( const QString code );
    Q_INVOKABLE QList<QString> getLanguageCodes() const;

    Q_INVOKABLE DateOrder getDateOrder();
    Q_INVOKABLE void setDateOrder( DateOrder dateOrder );

    Q_INVOKABLE DateOrder getDefaultDateOrder() const;
    Q_INVOKABLE DateTimeFormat getDefaultDateFormat() const;
    Q_INVOKABLE DateTimeFormat getDefaultTimeFormat() const;
    Q_INVOKABLE DateTimeFormat getDefaultDateTimeFormat() const;

    Q_INVOKABLE HourFormat getHourFormat();
    Q_INVOKABLE void setHourFormat( HourFormat hourFormat );

    //TODO locale-aware lessthan function appropriate for qSort (http://doc.qt.nokia.com/4.7/qtalgorithms.html#qSort-2)
    //TODO collators
protected:

    LocalePrivate* const d_ptr;

private:
    Q_DECLARE_PRIVATE(Locale);

};

} //namespace meego

#endif // MEEGO_LOCALE_H