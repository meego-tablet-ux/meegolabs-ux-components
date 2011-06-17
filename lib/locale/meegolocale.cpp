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
          mDateFormat(MDY),
          mHourFormat(Hrs12),
          mFirstDayOfWeek(DaySunday),
          mpQLocale(new QLocale),
          mpDefaultCollator(0),
          mpPhoneBookCollator(0)
    {
        //: read DateOrder
        //: translator: this order will determine order of date picker widgets
        //: default order for numeric date (m = month, d = date, y = year)
        //: the string should contain these exact three characters in some order
        QString dateFormat = tr("mdy", "dateorder");

        if (dateFormat == "dmy") {
            mDateFormat = Locale::DMY;
        }
        else if (dateFormat == "ymd") {
            mDateFormat = Locale::YMD;
        }
        else {
            mDateFormat = Locale::MDY;
        }

        //: read 24 hrs flag DateOrder
        //: translator: set to 1 for default 24-hour time format (0 for 12-hour)
        QString time24 = tr("0", "timeformat");
        if (time24 == "1") {
            mHourFormat = Locale::Hrs24;
        }
        else {
            mHourFormat = Locale::Hrs12;
        }

        //: read DateOfWeek
        //: translator: set to 1 for Monday, 2 for Tuesday, etc, and 7 for Sunday
        QString day = tr("1", "firstday");

        int dow = 1;
        if (1 == day.length()) {
            QChar ch = day.at(0);
            if (ch >= '1' && ch <= '7') {
                dow = ch.toAscii() - '0';
            }
        }

        mFirstDayOfWeek = (Locale::DayOfWeek)dow;

        mLocale = QLocale::system().name();
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
        return mDateFormat;
    }


    void Locale::setDateFormat(DateFormat newValue)
    {
        qWarning() << __FUNCTION__ << "not implemented yet!";
        mDateFormat = newValue;
        emit dateFormatChanged();
    }


    Locale::HourFormat Locale::hourFormat() const
    {
        return mHourFormat;
    }


    void Locale::setHourFormat(HourFormat newValue)
    {
        qWarning() << __FUNCTION__ << "not implemented yet!";
        mHourFormat = newValue;
        emit hourFormatChanged();
    }


    Locale::DayOfWeek Locale::firstDayOfWeek() const
    {
        return mFirstDayOfWeek;
    }


    void Locale::setFirstDayOfWeek(DayOfWeek newValue)
    {
        qWarning() << __FUNCTION__ << "not implemented yet!";
        mFirstDayOfWeek = newValue;
        emit firstDayOfWeekChanged();
    }


    QString Locale::localDate(const QDate &date, Locale::DateTimeFormat format) const
    {
        if (format <= DateBEGIN || format >= DateEND) {
            qWarning() << "Locale: invalid date format: " << format;
            return QString();
        }

        return date.toString( formatString(format) );
    }


    QString Locale::localTime(const QTime &time, Locale::DateTimeFormat format) const
    {
        if (format <= TimeBEGIN || format >= TimeEND) {
            qWarning() << "Locale: invalid time format: " << format;
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

        if (format == TimeFull) {
            if( mHourFormat == Hrs12 )
                format = TimeFull12;
            else
                format = TimeFull24;
        }

        switch (format) {
        case TimeFull12:
            //: AM time format string (translator: update order / format)
            //: %1 = hours (no leading zero), %2 = minutes (leading zero if needed), %3 = AM/PM string
            return tr("%1:%2 %3").arg(hour12).arg(minute, 2, 10, QChar('0')).arg(ampm);

        case TimeFull24:
            //: 24-hour time format string (translator: update order / format)
            //: %1 = hours (no leading zero), %2 = minutes (leading zero if needed)
            return tr("%1:%2").arg(hour).arg(minute, 2, 10, QChar('0'));

        default:
            qWarning() << "Locale: unhandled time format: " << format;
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


    QString Locale::decimalPoint() const
    {
        return mpQLocale->decimalPoint();
    }


    QList<QString> Locale::installedLocales() const
    {
        return QList<QString>(); //TODO where comes the codes from?
    }


    QString Locale::localeDisplayName(QString locale) const
    {
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
        switch (format) {
        case meego::Locale::DateFullLong:
            //: QDateTime format string (translator: update order / format) - See http://doc.qt.nokia.com/4.7/qdatetime.html#toString
            //: dddd = full day of week, MMMM = full month, d = day of month, yyyy = year (e.g. Monday, January 31, 2011)
            return tr("dddd, MMMM d, yyyy");

        case meego::Locale::DateFull:
            //: QDateTime format string (translator: update order / format)
            //: MMMM = full month, d = day of month, yyyy = full year (e.g. January 31, 2011)
            return tr("MMMM d, yyyy");

        case meego::Locale::DateFullShort:
            //: QDateTime format string (translator: update order / format)
            //: MMM = short month, d = day of month, yyyy = full year (e.g. Jan 31 2011)
            return tr("MMM d yyyy");

        case meego::Locale::DateFullNum:
            //: QDateTime format string (translator: update order / format)
            //: M = numeric month, d = day of month, yyyy = full year (e.g. 1/31/2011)
            return tr("M/d/yyyy");

         case meego::Locale::DateFullNumShort:
            //: QDateTime format string (translator: update order / format)
            //: M = numeric month, d = day of month, yy = year (e.g. 1/31/11)
            return tr("M/d/yy");

        case meego::Locale::DateWeekdayMonthDay:
            //: QDateTime format string (translator: update order / format)
            //: dddd = full day of week, MMMM = full month, d = day of month (e.g. Monday, January 31)
            return tr("dddd, MMMM d");

        case meego::Locale::DateWeekdayDayShort:
            //: QDateTime format string (translator: update order / format)
            //: ddd = short day of week, d = day of month (e.g. Mon 31)
            return tr("ddd d");

        case meego::Locale::DateMonthDay:
            //: QDateTime format string (translator: update order / format)
            //: MMMM = full month, d = day of month (e.g. January 31)
            return tr("MMMM d");

        case meego::Locale::DateMonthYear:
            //: QDateTime format string (translator: update order / format)
            //: MMMM = full month, yyyy = full year (e.g. January 2011)
            return tr("MMMM yyyy");

        case meego::Locale::DateMonthYearShort:
            //: QDateTime format string (translator: update order / format)
            //: MMM = short month, yyyy = full year (e.g. Jan 2011)
            return tr("MMM yyyy");

        case meego::Locale::DateDay:
            //: QDateTime format string (translator: you probably won't change this)
            //: d = day of month (e.g. 31, no leading zero)
            return tr("d");

        case meego::Locale::DateWeekday:
            //: QDateTime format string (translator: you probably won't change this)
            //: dddd = full day of week (e.g. Monday)
            return tr("dddd");

        case meego::Locale::DateWeekdayShort:
            //: QDateTime format string (translator: you probably won't change this)
            //: ddd = short day of week (e.g. Mon)
            return tr("ddd");

        case meego::Locale::DateMonth:
            //: QDateTime format string (translator: you probably won't change this)
            //: MMMM = full month (e.g. January)
            return tr("MMMM");

        case meego::Locale::DateMonthShort:
            //: QDateTime format string (translator: you probably won't change this)
            //: MMM = short month (e.g. Jan)
            return tr("MMM");

        case meego::Locale::DateYear:
            //: QDateTime format string (translator: you probably won't change this)
            //: yyyy (e.g. 2011)
            return tr("yyyy");

        default:
            qWarning() << "Locale: unexpected format string";
            return QString();
        }
    }

} //namespace meego
