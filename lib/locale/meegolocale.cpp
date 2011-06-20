/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "meegolocale.h"

#include <QDate>
#include <QDateTime>
#include <QTime>
#include <QLocale>

#include <unicode/unistr.h>
#include <unicode/locid.h>
#include <unicode/coll.h>
#include <unicode/tblcoll.h>
#include <unicode/uchar.h>
#include <unicode/ulocdata.h>
#include <unicode/ustring.h>
#include <unicode/uchriter.h>

#include <QDebug>


namespace
{
    QString toQString(const icu::UnicodeString & str)
    {
        const UChar * buf = str.getBuffer();
        return QString(reinterpret_cast<const QChar *>(buf), str.length());
    }

    icu::UnicodeString toUnicodeString(const QString & str)
    {
        const ushort *strShort = str.utf16();
        return UnicodeString(static_cast<const UChar *>(strShort));
    }


    enum CollationTypes {
        Default = 0,
        PhoneBook,
        Pinyin,
        Traditional,
        Stroke,
        Direct
    };


    bool compare (const QString & lStr, const QString & rStr, icu::Collator & coll)
    {
        UnicodeString lUniStr = toUnicodeString(lStr);
        UnicodeString rUniStr = toUnicodeString(rStr);

        icu::Collator::EComparisonResult result = coll.compare(lUniStr, rUniStr);

        switch (result) {
          case icu::Collator::LESS :
              return -1;
              break;
          case icu::Collator::GREATER :
              return 1;
              break;
          default :
              return 0;
        };
    }


    QLocale::Country country()
    {
        return QLocale::system().country();
    }


    icu::Collator * createCollator (int collType, QString locale, QLocale::Country country)
    {
        QString collation;

        switch (collType) {
          case PhoneBook:
              collation = "@collation=phonebook";
              break;
          case Pinyin:
              collation = "@collation=pinyin";
              break;
          case Traditional:
              collation = "@collation=traditional";
              break;
          case Stroke:
              collation = "@collation=stroke";
              break;
          case Direct:
              collation = "@collation=direct";
              break;
          default:
              collation = "@collation=default";
        }

        const char *name = (locale + collation).toLatin1().constData();
        icu::Locale localeName = icu::Locale(name);

        UErrorCode status = U_ZERO_ERROR;
        icu::Collator * pColl = icu::Collator::createInstance(localeName, status);
        
        if (U_SUCCESS(status) &&
            ((country == QLocale::DemocraticRepublicOfKorea) ||
             (country == QLocale::RepublicOfKorea) ||
             (country == QLocale::Japan))) {

            //ASCII characters should be sorted after
            //non-ASCII characters for some languages
            UnicodeString rules = ((icu::RuleBasedCollator *)pColl)->getRules();
            rules += "< a,A< b,B< c,C< d,D< e,E< f,F< g,G< h,H< i,I< j,J < k,K"
                "< l,L< m,M< n,N< o,O< p,P< q,Q< r,R< s,S< t,T < u,U< v,V"
                "< w,W< x,X< y,Y< z,Z";

            delete pColl;
            pColl = new icu::RuleBasedCollator(rules, status);
        }

        if (!U_SUCCESS(status)) {
            delete pColl;
            pColl = 0;
        }
        return pColl;
    }
    
} // anonymous namespace


namespace meego
{

    Locale::Locale(QObject *parent)
        : QObject(parent),
          mpQLocale(new QLocale()),
          mDateFormat(defaultDateFormat()),
          mTimeFormat(defaultTimeFormat()),
          mFirstDayOfWeek(defaultFirstDayOfWeek()),
          mDecimalPoint(defaultDecimalPoint()),
          mpDefaultCollator(0),
          mpPhoneBookCollator(0)
    {
        mLocale = QLocale::system().name();
    }

    Locale::DateFormat Locale::defaultDateFormat() const
    {
        // read DateFormat
        //: this string defines default order for dates
        //: choose one of "dmy", "ymd", or "mdy" (m = month, d = date, y = year)
        QString dateFormat = tr("mdy", "dateformat");

        if (dateFormat == "dmy") {
            return DateFormatDMY;
        }
        else if (dateFormat == "ymd") {
            return DateFormatYMD;
        }
        else {
            return DateFormatMDY;
        }
    }

    Locale::TimeFormat Locale::defaultTimeFormat() const
    {
        // read 24 hrs flag TimeFormat
        //: set to 1 for default 24-hour time format (0 for 12-hour)
        QString time24 = tr("0", "timeformat");
        if (time24 == "1") {
            return TimeFormat24;
        }
        else {
            return TimeFormat12;
        }
    }

    Locale::DayOfWeek Locale::defaultFirstDayOfWeek() const
    {
        // read DayOfWeek
        //: this number sets default first day of week
        //: set to 1 for Monday, 2 for Tuesday, etc, and 7 for Sunday
        QString day = tr("7", "firstday");

        int dow = 7;
        if (1 == day.length()) {
            QChar ch = day.at(0);
            if (ch >= '1' && ch <= '7') {
                dow = ch.toAscii() - '0';
            }
        }

        return (DayOfWeek)dow;
    }

    
    Locale::~Locale()
    {
        delete mpQLocale;
        delete mpDefaultCollator;
        delete mpPhoneBookCollator;
    }
    

    QString Locale::locale() const
    {
        return mLocale;
    }


    void Locale::setLocale(QString newValue)
    {
        qWarning() << __FUNCTION__ << "not implemented yet!";
        mLocale = newValue;
        emit localeChanged();
    }


    Locale::DateFormat Locale::dateFormat() const
    {
        // Read possible user override from settings
        return defaultDateFormat();
    }


    void Locale::setDateFormat(DateFormat)
    {
        qWarning() << __FUNCTION__ << "not implemented yet!";
    }


    Locale::TimeFormat Locale::timeFormat() const
    {
        // TODO: Read possible user override from settings
        return defaultTimeFormat();
    }


    void Locale::setTimeFormat(TimeFormat)
    {
        qWarning() << __FUNCTION__ << "not implemented yet!";
    }


    Locale::DayOfWeek Locale::firstDayOfWeek() const
    {
        // TODO: Read possible user override from settings
        return defaultFirstDayOfWeek();
    }


    void Locale::setFirstDayOfWeek(DayOfWeek)
    {
        qWarning() << __FUNCTION__ << "not implemented yet!";
    }


    QString Locale::localDate(const QDate &date, Locale::DateTimeFormat format) const
    {
        if (format <= DateBEGIN || format >= DateEND) {
            qWarning() << "meego::Locale: invalid date format: " << format;
            return QString();
        }

        return date.toString( formatString(format) );
    }


    QString Locale::localTime(const QTime &time, Locale::DateTimeFormat format) const
    {
        if (format <= TimeBEGIN || format >= TimeEND) {
            qWarning() << "meego::Locale: invalid time format: " << format;
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

        if (format == TimeFull)
            format = (timeFormat() == TimeFormat24) ? TimeFull24 : TimeFull12;

        switch (format) {
        case TimeFull12:
            //: AM time format string (update format / order )
            //: %1 = hours (no leading zero), %2 = minutes (leading zero if needed), %3 = AM/PM string
            return tr("%1:%2 %3").arg(hour12).arg(minute, 2, 10, QChar('0')).arg(ampm);

        case TimeFull24:
            //: 24-hour time format string (update format / order)
            //: %1 = hours (no leading zero), %2 = minutes (leading zero if needed)
            return tr("%1:%2").arg(hour).arg(minute, 2, 10, QChar('0'));

        default:
            qWarning() << "LocaleHelper: unhandled time format: " << format;
            return QString();
        }
    }


    QString Locale::currentDate(Locale::DateTimeFormat format) const
    {
        return localDate(QDate::currentDate(), format);
    }


    QString Locale::currentTime(Locale::DateTimeFormat format) const
    {
        return localTime(QTime::currentTime(), format);
    }


    QString Locale::defaultDecimalPoint() const
    {
        return mpQLocale->decimalPoint();
    }

    QString Locale::decimalPoint() const
    {
        // TODO: Read possible user override from settings
        return defaultDecimalPoint();
    }

    void Locale::setDecimalPoint( QString )
    {
        qWarning() << __FUNCTION__ << "not implemented yet!";
    }


    QString Locale::localeDisplayName(QString locale)
    {
        qDebug() << __FUNCTION__ << locale;

        icu::UnicodeString localeUCS = toUnicodeString(locale);
        icu::Locale displayLocale(locale.toLatin1());
        return toQString(displayLocale.getDisplayName(displayLocale, localeUCS));
    }

    
    bool Locale::lessThan(const QString &lStr, const QString &rStr) const
    {
        return (-1 == compare(lStr, rStr));
    }
    

    int  Locale::compare(const QString &lStr, const QString &rStr) const
    {
        if (! mpDefaultCollator) {
            mpDefaultCollator = createCollator(Default, locale(), country());
            if (!mpDefaultCollator) return 0;
        }
        return ::compare(lStr, rStr, *mpDefaultCollator);
    }
    

    bool Locale::lessThanPhoneBook(const QString &lStr, const QString &rStr) const
    {
        return (-1 == comparePhoneBook(lStr, rStr));
    }
    

    int  Locale::comparePhoneBook(const QString &lStr, const QString &rStr) const
    {
        if (! mpPhoneBookCollator) {
            mpPhoneBookCollator = createCollator(PhoneBook, locale(), country());
            if (!mpPhoneBookCollator) return 0;
        }
        return ::compare(lStr, rStr, *mpPhoneBookCollator);
    }
    

    QString Locale::formatString(Locale::DateTimeFormat format) const
    {
        DateFormat order = dateFormat();

        switch (format) {
        case DateFullLong:
            if (order == DateFormatDMY)
                //: QDateTime format string - See http://doc.qt.nokia.com/4.7/qdatetime.html#toString
                //: translator: long date in DMY order
                //: dddd = full day of week, MMMM = full month, d = day of month, yyyy = year (e.g. Monday, 31 January 2011)
                return tr("dddd, d MMMM yyyy");
            if (order == DateFormatYMD)
                //: translator: long date in YMD order (QDateTime format string)
                //: dddd = full day of week, MMMM = full month, d = day of month, yyyy = year (e.g. Monday, 2011 January 31)
                return tr("dddd, yyyy MMMM d");
            //: translator: long date in MDY order (QDateTime format string)
            //: dddd = full day of week, MMMM = full month, d = day of month, yyyy = year (e.g. Monday, January 31, 2011)
            return tr("dddd, MMMM d, yyyy");

        case DateFull:
            if (order == DateFormatDMY)
                //: translator: full date in DMY order (QDateTime format string)
                //: MMMM = full month, d = day of month, yyyy = full year (e.g. 31 January 2011)
                return tr("d MMMM yyyy");
            if (order == DateFormatYMD)
                //: translator: full date in YMD order (QDateTime format string)
                //: MMMM = full month, d = day of month, yyyy = full year (e.g. 2011 January 31)
                return tr("yyyy MMMM d");
            //: translator: full date in MDY order (QDateTime format string)
            //: MMMM = full month, d = day of month, yyyy = full year (e.g. January 31, 2011)
            return tr("MMMM d, yyyy");

        case DateFullShort:
            if (order == DateFormatDMY)
                //: translator: short date in DMY order (QDateTime format string)
                //: MMM = short month, d = day of month, yyyy = full year (e.g. 31 Jan 2011)
                return tr("d MMM d yyyy");
            if (order == DateFormatYMD)
                //: translator: short date in YMD order (QDateTime format string)
                //: MMM = short month, d = day of month, yyyy = full year (e.g. Jan 31 2011)
                return tr("MMM d yyyy");
            //: translator: short date in MDY order (QDateTime format string)
            //: MMM = short month, d = day of month, yyyy = full year (e.g. Jan 31 2011)
            return tr("MMM d yyyy");

        case DateFullNum:
            if (order == DateFormatDMY)
                //: translator: numeric date in DMY order (QDateTime format string)
                //: M = numeric month, d = day of month, yyyy = full year (e.g. 31/1/2011)
                return tr("d/M/yyyy");
            if (order == DateFormatYMD)
                //: translator: numeric date in YMD order (QDateTime format string)
                //: M = numeric month, d = day of month, yyyy = full year (e.g. 2011/1/31)
                return tr("yyyy/M/d");
            //: translator: numeric date in MDY order (QDateTime format string)
            //: M = numeric month, d = day of month, yyyy = full year (e.g. 1/31/2011)
            return tr("M/d/yyyy");

         case DateFullNumShort:
            if (order == DateFormatDMY)
                //: translator: short numeric date in DMY order (QDateTime format string)
                //: M = numeric month, d = day of month, yy = year (e.g. 31/1/11)
                return tr("d/M/yy");
            if (order == DateFormatYMD)
                //: translator: short numeric date in YMD order (QDateTime format string)
                //: M = numeric month, d = day of month, yy = year (e.g. 11/1/31)
                return tr("yy/M/d");
            //: translator: short numeric date in MDY order (QDateTime format string)
            //: M = numeric month, d = day of month, yy = year (e.g. 1/31/11)
            return tr("M/d/yy");

        case DateWeekdayMonthDay:
            // NOTE: here YMD and MDY are the same, so we pass tr() a context string
            //   in case they need to differ in some translation

            if (order == DateFormatDMY)
                //: translator: weekday, month, and day in DMY order (QDateTime format string)
                //: dddd = full day of week, MMMM = full month, d = day of month (e.g. Monday, 31 January)
                return tr("dddd, d MMMM");
            if (order == DateFormatYMD)
                //: translator: weekday, month, and day in DMY order (QDateTime format string)
                //: dddd = full day of week, MMMM = full month, d = day of month (e.g. Monday, 31 January)
                return tr("dddd, MMMM d", "YMD");
            //: translator: weekday, month, and day in YDM or MDY order (QDateTime format string)
            //: dddd = full day of week, MMMM = full month, d = day of month (e.g. Monday, January 31)
            return tr("dddd, MMMM d", "MDY");

        case DateWeekdayDayShort:
            // NOTE: here all cases are the samee, so we pass tr() a context string
            //   in case they need to differ in some translation

            if (order == DateFormatDMY)
                //: translator: weekday and day in DMY order (QDateTime format string)
                //: ddd = short day of week, d = day of month (e.g. Mon 31)
                return tr("ddd d", "DMY");
            if (order == DateFormatYMD)
                //: translator: weekday and day in DMY order (QDateTime format string)
                //: ddd = short day of week, d = day of month (e.g. Mon 31)
                return tr("ddd d", "YMD");
            //: translator: weekday and day in DMY order (QDateTime format string)
            //: ddd = short day of week, d = day of month (e.g. Mon 31)
            return tr("ddd d", "MDY");

        case DateMonthDay:
            // NOTE: here YMD and MDY are the same, so we pass tr() a context string
            //   in case they need to differ in some translation

            if (order == DateFormatDMY)
                //: translator: month and day in DMY order (QDateTime format string)
                //: MMMM = full month, d = day of month (e.g. 31 January)
                return tr("d MMMM");
            if (order == DateFormatYMD)
                //: translator: month and day in YMD order (QDateTime format string)
                //: MMMM = full month, d = day of month (e.g. January 31)
                return tr("MMMM d", "YMD");
            //: translator: month and day in MDY order (QDateTime format string)
            //: MMMM = full month, d = day of month (e.g. January 31)
            return tr("MMMM d", "MDY");

        case DateMonthYear:
            // NOTE: here DMY and MDY are the same, so we pass tr() a context string
            //   in case they need to differ in some translation

            if (order == DateFormatDMY)
                //: translator: month and year in DMY order (QDateTime format string)
                //: MMMM = full month, yyyy = full year (e.g. January 2011)
                return tr("MMMM yyyy", "DMY");
            if (order == DateFormatYMD)
                //: translator: month and year in YMD order (QDateTime format string)
                //: MMMM = full month, yyyy = full year (e.g. 2011 January)
                return tr("yyyy MMMM");
            //: translator: month and year in MDY order (QDateTime format string)
            //: MMMM = full month, yyyy = full year (e.g. January 2011)
            return tr("MMMM yyyy", "MDY");

        case DateMonthYearShort:
            // NOTE: here DMY and MDY are the same, so we pass tr() a context string
            //   in case they need to differ in some translation

            if (order == DateFormatDMY)
                //: translator: short month and year in DMY order (QDateTime format string)
                //: MMM = short month, yyyy = full year (e.g. Jan 2011)
                return tr("MMM yyyy", "DMY");
            if (order == DateFormatYMD)
                //: QDateTime format string (translator: update order / format)
                //: MMM = short month, yyyy = full year (e.g. 2011 Jan)
                return tr("yyyy MMM");
            //: QDateTime format string (translator: update order / format)
            //: MMM = short month, yyyy = full year (e.g. Jan 2011)
            return tr("MMM yyyy", "MDY");

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

        default:
            qWarning() << "meego::Locale: unexpected format string";
            return QString();
        }
    }

} //namespace meego
