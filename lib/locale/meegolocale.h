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
#include <MGConfItem>
#include <QLocale>

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
        Q_ENUMS( TimeFormat );

        Q_PROPERTY( QString     locale                READ locale WRITE setLocale NOTIFY localeChanged );
        Q_PROPERTY( QLocale::Country country          READ country NOTIFY countryChanged );
        Q_PROPERTY( DateFormat  dateFormat            READ dateFormat WRITE setDateFormat NOTIFY dateFormatChanged );
        Q_PROPERTY( DateFormat  defaultDateFormat     READ defaultDateFormat NOTIFY defaultDateFormatChanged );
        Q_PROPERTY( TimeFormat  timeFormat            READ timeFormat WRITE setTimeFormat NOTIFY timeFormatChanged );
        Q_PROPERTY( TimeFormat  defaultTimeFormat     READ defaultTimeFormat NOTIFY defaultTimeFormatChanged );
        Q_PROPERTY( DayOfWeek   firstDayOfWeek        READ firstDayOfWeek WRITE setFirstDayOfWeek NOTIFY firstDayOfWeekChanged );
        Q_PROPERTY( DayOfWeek   defaultFirstDayOfWeek READ defaultFirstDayOfWeek NOTIFY defaultFirstDayOfWeekChanged );
        Q_PROPERTY( QString     decimalPoint          READ decimalPoint WRITE setDecimalPoint NOTIFY decimalPointChanged)
        Q_PROPERTY( QString     defaultDecimalPoint   READ defaultDecimalPoint NOTIFY defaultDecimalPointChanged)

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
            DayInvalid = 0,
            DayMonday = 1,  // Matches QLocale::dayName() numbering
            DayTuesday,
            DayWednesday,
            DayThursday,
            DayFriday,
            DaySaturday,
            DaySunday
        };
        
        enum DateFormat {
            DateFormatInvalid = -1,
            DateFormatYMD = 0,
            DateFormatDMY,
            DateFormatMDY
        };
        
        enum TimeFormat {
            TimeFormatInvalid = -1,
            TimeFormat12 = 0,
            TimeFormat24
        };

        QString locale() const;
        void setLocale( QString );

        TimeFormat  timeFormat() const;
        void        setTimeFormat( TimeFormat );
        void        resetTimeFormat();
        TimeFormat defaultTimeFormat() const;

        // Returns a three-character string with the letters 'd', 'm', and 'y' in
        //   the order that those should appear for this locale, e.g. "mdy" for
        //   American 1/31/2011, "dmy" for European 31/1/2011
        DateFormat dateFormat() const;
        void       setDateFormat( DateFormat );
        void       resetDateFormat();
        DateFormat defaultDateFormat() const;

        // localized first day of the week (returns DayOfWeek enum)
        DayOfWeek firstDayOfWeek() const;
        void      setFirstDayOfWeek( DayOfWeek dayofWeek );
        void      resetFirstDayOfWeek();
        DayOfWeek defaultFirstDayOfWeek() const;

        QLocale::Country country() const;

        // date/time formatting
        Q_INVOKABLE QString localDate(const QDate &date, DateTimeFormat format) const;
        Q_INVOKABLE QString localTime(const QTime &time, DateTimeFormat format) const;
        Q_INVOKABLE QString currentDate(DateTimeFormat format) const;
        Q_INVOKABLE QString currentTime(DateTimeFormat format) const;

        // localized decimal point
        Q_INVOKABLE QString decimalPoint() const;
        Q_INVOKABLE void setDecimalPoint( QString );
        Q_INVOKABLE void resetDecimalPoint();
        Q_INVOKABLE QString defaultDecimalPoint() const;

        Q_INVOKABLE static QString localeDisplayName( QString locale );

        bool lessThan(const QString & lStr, const QString & rStr) const;
        int  compare(const QString & lStr, const QString & rStr) const;
        bool lessThanPhoneBook(const QString & lStr, const QString & rStr) const;
        int  comparePhoneBook(const QString & lStr, const QString & rStr) const;

      signals:

        void localeChanged();
        void dateFormatChanged();
        void defaultDateFormatChanged();
        void timeFormatChanged();
        void defaultTimeFormatChanged();
        void firstDayOfWeekChanged();
        void defaultFirstDayOfWeekChanged();
        void decimalPointChanged();
        void defaultDecimalPointChanged();
        void countryChanged();
        
      protected:

        QString formatString(DateTimeFormat) const;

      private slots:
        void readLocaleConfItem();
        void readDateFormatConfItem();
        void readTimeFormatConfItem();
        void readDecimalPointConfItem();
        void readFirstWeekConfItem();

      private:
        
        QLocale    mQLocale;
        DateFormat mDateFormat;
        TimeFormat mTimeFormat;
        DayOfWeek  mFirstDayOfWeek;
        QString    mDecimalPoint;        
        QString    mLocale;

        mutable icu::Collator * mpDefaultCollator;
        mutable icu::Collator * mpPhoneBookCollator;

        MGConfItem mLocaleConfItem;
        MGConfItem mDateFormatConfItem;
        MGConfItem mTimeFormatConfItem;
        MGConfItem mDecimalPointConfItem;
        MGConfItem mFirstDayOfWeekConfItem;

    };
    
} //namespace meego

#endif // MEEGO_LOCALE_H
