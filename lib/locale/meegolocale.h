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
#include <QList>


class QLocale;
class QDate;
class QTime;

namespace icu {
    class Collator;
}

namespace meego
{

    // This is a class for providing locale-related information

    class LOCALESHARED_EXPORT Locale : public QObject
    {
        Q_OBJECT;

        Q_ENUMS( DayOfWeek );
        Q_ENUMS( DateTimeFormat );
        Q_ENUMS( DateFormat );
        Q_ENUMS( HourFormat );

        Q_PROPERTY( QString    locale         READ locale WRITE setLocale NOTIFY localeChanged );
        Q_PROPERTY( DateFormat dateFormat     READ dateFormat WRITE setDateFormat NOTIFY dateFormatChanged );
        Q_PROPERTY( HourFormat hourFormat     READ hourFormat WRITE setHourFormat NOTIFY hourFormatChanged );
        Q_PROPERTY( DayOfWeek  firstDayOfWeek READ firstDayOfWeek WRITE setFirstDayOfWeek NOTIFY firstDayOfWeekChanged );

      public:
        explicit Locale (QObject *parent = 0);
        ~Locale();
        
        enum DateTimeFormat
        {
            // Date formats

            DateBEGIN = 100,

            DateFullLong,         // Monday, January 31, 2011
            DateFull,             // January 31, 2011
            DateFullShort,        // Jan 31 2011
            DateFullNum,          // 1/31/2011
            DateFullNumShort,     // 1/31/11
            DateWeekdayMonthDay,  // Monday, January 31
            DateMonthDay,         // January 31
            DateMonthYear,        // January 2011
            DateMonthYearShort,   // Jan 2011
            DateDay,              // 31 (no leading zero)
            DateWeekday,          // Monday
            DateWeekdayShort,     // Mon
            DateWeekdayDayShort,  // Mon 31
            DateMonth,            // January
            DateMonthShort,       // Jan
            DateYear,             // 2011

            DateEND,

            // Time formats

            TimeBEGIN = 200,

            TimeFull,    // 9:02 PM or 21:02
            TimeFull12,  // 9:02 PM
            TimeFull24,  // 21:02

            TimeEND,
        };
        
        enum DayOfWeek {
            DayMonday = 1,  // Matches QLocale numbering
            DayTuesday,
            DayWednesday,
            DayThursday,
            DayFriday,
            DaySaturday,
            DaySunday
        };
        
        enum DateFormat {
            YMD = 0,
            DMY,
            MDY
        };
        
        enum HourFormat {
            Hrs12 = 0,
            Hrs24
        };

        QString locale() const;
        void setLocale( QString );

        HourFormat hourFormat() const;
        void setHourFormat( HourFormat );

        // Returns a three-character string with the letters 'd', 'm', and 'y' in
        //   the order that those should appear for this locale, e.g. "mdy" for
        //   American 1/31/2011, "dmy" for European 31/1/2011
        DateFormat dateFormat() const;
        void       setDateFormat( DateFormat );

        // localized first day of the week (returns DayOfWeek enum)
        DayOfWeek firstDayOfWeek() const;
        void      setFirstDayOfWeek( DayOfWeek dayofWeek );

        // date/time formatting
        Q_INVOKABLE QString localDate(const QDate &date, DateTimeFormat format) const;
        Q_INVOKABLE QString localTime(const QTime &time, DateTimeFormat format) const;
        Q_INVOKABLE QString currentDate(DateTimeFormat format) const;
        Q_INVOKABLE QString currentTime(DateTimeFormat format) const;

        // localized decimal point
        Q_INVOKABLE QString decimalPoint() const;

        Q_INVOKABLE QList<QString> installedLocales() const;
        Q_INVOKABLE QString localeDisplayName(QString locale) const;


        bool lessThan(const QString & lStr, const QString & rStr) const;
        int  compare(const QString & lStr, const QString & rStr) const;
        bool lessThanPhoneBook(const QString & lStr, const QString & rStr) const;
        int  comparePhoneBook(const QString & lStr, const QString & rStr) const;

      signals:

        void localeChanged();
        void numericDateOrderChanged();
        void hourFormatChanged();
        void dateFormatChanged();
        void firstDayOfWeekChanged();
        void installedLocalesChanged();
        
      protected:

        QString formatString(DateTimeFormat) const;

      private:
        
        DateFormat mDateFormat;
        HourFormat mHourFormat;
        DayOfWeek  mFirstDayOfWeek;
        QString    mLocale;
        QLocale  * mpQLocale;

        mutable icu::Collator * mpDefaultCollator;
        mutable icu::Collator * mpPhoneBookCollator;
    };
    
} //namespace meego

#endif // MEEGO_LOCALE_H
