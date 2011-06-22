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
#include <QFile>
#include <QDir>

#include <unicode/unistr.h>
#include <unicode/locid.h>
#include <unicode/coll.h>
#include <unicode/tblcoll.h>
#include <unicode/uchar.h>
#include <unicode/ulocdata.h>
#include <unicode/ustring.h>
#include <unicode/uchriter.h>

#include <QDebug>
#include <QSettings>

#include "mgconfitem.h"

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


    int compare (const QString & lStr, const QString & rStr, icu::Collator & coll)
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

    QStringList getLanguageList()
    {
        QStringList languageList;
        for(int i=0; i<215; i++)
        {
            QString language = QLocale::languageToString((QLocale::Language)i);
            if(language != "")
            {
                foreach(QLocale::Country country,QLocale::countriesForLanguage((QLocale::Language)i))
                {
                    QString localeString = language + " - " + QLocale::countryToString(country);
                    languageList<<localeString;
                }
            }
        }
        return languageList;
    }

    void setLocaleToQSettings(QString locale)
    {
        if(!QFile::exists(QDir::homePath() + "/.config/sysconfig/i18n"))
        {
            if(!QDir::home().exists(QDir::homePath() + "/.config/sysconfig"))
            {
                QDir::home().mkpath(".config/sysconfig");
            }
            QFile::copy("/etc/sysconfig/i18n", QDir::homePath() + "/.config/sysconfig/i18n");
        }
        QSettings i18n(QDir::homePath() + "/.config/sysconfig/i18n", QSettings::NativeFormat);
        i18n.setValue("LANG", locale + ".UTF-8");
    }
    void setLocaleEnvironment(QString locale)
    {
        //nop
    }


} // anonymous namespace


namespace meego
{

    Locale::Locale(QObject *parent)
        : QObject(parent),
          mpQLocale(new QLocale()),
          mDateFormat(DateFormatInvalid),
          mTimeFormat(TimeFormatInvalid),
          mFirstDayOfWeek(DayInvalid),
          mDecimalPoint(""),
          mpDefaultCollator(0),
          mpPhoneBookCollator(0),
          mLanguageConfItem( LANG_KEY ),
          mDateFormatConfItem( DATE_KEY ),
          mTimeFormatConfItem( TIME_KEY ),
          mDecimalPointConfItem( DEC_KEY ),
          mFirstDayOfWeekConfItem( DAY_KEY )
    {

        connect( &mLanguageConfItem, SIGNAL( valueChanged() ), this, SLOT( readLanguageConfItem() ) );
        connect( &mDateFormatConfItem, SIGNAL( valueChanged() ), this, SLOT( readDateFormatConfItem() ) );
        connect( &mTimeFormatConfItem, SIGNAL( valueChanged() ), this, SLOT( readTimeFormatConfItem() ) );
        connect( &mDecimalPointConfItem, SIGNAL( valueChanged() ), this, SLOT( readDecimalConfItem() ) );
        connect( &mFirstDayOfWeekConfItem, SIGNAL( valueChanged() ), this, SLOT( readFirstWeekConfItem() ) );

        readLanguageConfItem();
        readDateFormatConfItem();
        readTimeFormatConfItem();
        readDecimalPointConfItem();
        readFirstWeekConfItem();

        //TODO from confitem?
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

    QString Locale::defaultDecimalPoint() const
    {
        return mpQLocale->decimalPoint();
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


    void Locale::setLocale( QString v )
    {
        mLocale = v;
        if( getLanguageList().contains(v) )
        {
            setLocaleToQSettings(v);
            setLocaleEnvironment(v);
            mLanguageConfItem.set( QVariant(v) );
        }
        emit localeChanged();
    }

    Locale::DateFormat Locale::dateFormat() const
    {
        return mDateFormat;
    }


    void Locale::setDateFormat( DateFormat v )
    {        
        mDateFormat = v;

        if( v != DateFormatInvalid)
        {
            if( v == DateFormatYMD )
                mDateFormatConfItem.set( QVariant( "ymd" ) );
            else if( v == DateFormatDMY )
                mDateFormatConfItem.set( QVariant( "dmy" ) );
            else if( v == DateFormatMDY )
                mDateFormatConfItem.set( QVariant( "mdy" ) );
        } else {
            mDateFormatConfItem.set( QVariant( "invalid" ) );
        }

        emit dateFormatChanged();
    }


    void Locale::resetDateFormat() {
        mDateFormatConfItem.set( QVariant("") );
        mDateFormat = defaultDateFormat();
        emit dateFormatChanged();
    }


    Locale::TimeFormat Locale::timeFormat() const
    {
        return mTimeFormat;
    }


    void Locale::setTimeFormat( TimeFormat v )
    {
        mTimeFormat = v;
        if( TimeFormat12 == v )
            mTimeFormatConfItem.set( QVariant( 12 ) );
        else if ( TimeFormat24 == v)
            mTimeFormatConfItem.set( QVariant( 24 ) );
        else
            mTimeFormatConfItem.set( QVariant( 0 ) );

        emit timeFormatChanged();
    }


    void Locale::resetTimeFormat() {
        mTimeFormatConfItem.set( QVariant("") );
        mTimeFormat = defaultTimeFormat();
        emit timeFormatChanged();
    }


    Locale::DayOfWeek Locale::firstDayOfWeek() const
    {
        return mFirstDayOfWeek;
    }


    void Locale::setFirstDayOfWeek( DayOfWeek v )
    {
        mFirstDayOfWeek = v;

        QChar ch = '0' + (int)v;
        if (ch >= '1' && ch <= '7')
            ch = '0';

        mFirstDayOfWeekConfItem.set( QVariant( QString( ch ) ) );

        emit firstDayOfWeekChanged();
    }


    void Locale::resetFirstDayOfWeek()
    {
        mFirstDayOfWeekConfItem.set( QVariant("") );
        mFirstDayOfWeek = defaultFirstDayOfWeek();
        emit firstDayOfWeekChanged();
    }



    QString Locale::decimalPoint() const
    {
        return mDecimalPoint;
    }


    void Locale::setDecimalPoint( QString v )
    {
        mDecimalPoint = v;
        mDecimalPointConfItem.set( QVariant( v ) );
        emit decimalPointChanged();
    }


    void Locale::resetDecimalPoint()
    {
        mDecimalPointConfItem.set( QVariant( "" ) );
        mDecimalPoint = defaultDecimalPoint();
        emit decimalPointChanged();
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


    QString Locale::localeDisplayName(QString locale)
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
        DateFormat order = dateFormat();

        switch (format) {
        case DateFullLong:
            if (order == DateFormatDMY)
                //: long date where user chose DMY order (e.g. Monday, 31 January 2011)
                //: Translate the format delimiters, not order (except you can move dddd here)
                //: dddd = full day of week, MMMM = full month, d = day of month, yyyy = year (QDateTime format string)
                //: See http://doc.qt.nokia.com/4.7/qdatetime.html#toString
                return tr("dddd, d MMMM yyyy");
            if (order == DateFormatYMD)
                //: long date where user chose YMD order (e.g. Monday, 2011 January 31)
                //: Translate the format delimiters, not order (except you can move dddd here)
                //: dddd = full day of week, MMMM = full month, d = day of month, yyyy = year (QDateTime format string)
                return tr("dddd, yyyy MMMM d");
            //: long date where user chose MDY order (e.g. Monday, January 31, 2011)
            //: Translate the format delimiters, not order (except you can move dddd here)
            //: dddd = full day of week, MMMM = full month, d = day of month, yyyy = year (QDateTime format string)
            return tr("dddd, MMMM d, yyyy");

        case DateFull:
            if (order == DateFormatDMY)
                //: full date where user chose DMY order (e.g. 31 January 2011)
                //: Translate the format delimiters, not order
                //: MMMM = full month, d = day of month, yyyy = full year (QDateTime format string)
                return tr("d MMMM yyyy");
            if (order == DateFormatYMD)
                //: full date where user chose YMD order (QDateTime format string)
                //: Translate the format delimiters, not order
                //: MMMM = full month, d = day of month, yyyy = full year (e.g. 2011 January 31)
                return tr("yyyy MMMM d");
            //: full date where user chose MDY order (QDateTime format string)
            //: Translate the format delimiters, not order
            //: MMMM = full month, d = day of month, yyyy = full year (e.g. January 31, 2011)
            return tr("MMMM d, yyyy");

        case DateFullShort:
            if (order == DateFormatDMY)
                //: short date where user chose DMY order (e.g. 31 Jan 2011)
                //: Translate the format delimiters, not order
                //: MMM = short month, d = day of month, yyyy = full year (QDateTime format string)
                return tr("d MMM yyyy");
            if (order == DateFormatYMD)
                //: short date where user chose YMD order (e.g. 2011 Jan 31)
                //: Translate the format delimiters, not order
                //: MMM = short month, d = day of month, yyyy = full year (QDateTime format string)
                return tr("yyyy MMM d");
            //: short date where user chose MDY order (e.g. Jan 31 2011)
            //: Translate the format delimiters, not order
            //: MMM = short month, d = day of month, yyyy = full year (QDateTime format string)
            return tr("MMM d yyyy");

        case DateFullNum:
            if (order == DateFormatDMY)
                //: numeric date where user chose DMY order (e.g. 31/1/2011)
                //: Translate the format delimiters, not order
                //: M = numeric month, d = day of month, yyyy = full year (QDateTime format string)
                return tr("d/M/yyyy");
            if (order == DateFormatYMD)
                //: numeric date where user chose YMD order (e.g. 2011/1/31)
                //: Translate the format delimiters, not order
                //: M = numeric month, d = day of month, yyyy = full year (QDateTime format string)
                return tr("yyyy/M/d");
            //: numeric date where user chose MDY order (e.g. 1/31/2011)
            //: Translate the format delimiters, not order
            //: M = numeric month, d = day of month, yyyy = full year (QDateTime format string)
            return tr("M/d/yyyy");

         case DateFullNumShort:
            if (order == DateFormatDMY)
                //: short numeric date where user chose DMY order (e.g. 31/1/11)
                //: Translate the format delimiters, not order
                //: M = numeric month, d = day of month, yy = year (QDateTime format string)
                return tr("d/M/yy");
            if (order == DateFormatYMD)
                //: short numeric date where user chose YMD order (e.g. 11/1/31)
                //: Translate the format delimiters, not order
                //: M = numeric month, d = day of month, yy = year (QDateTime format string)
                return tr("yy/M/d");
            //: short numeric date where user chose MDY order (e.g. 1/31/11)
            //: Translate the format delimiters, not order
            //: M = numeric month, d = day of month, yy = year (QDateTime format string)
            return tr("M/d/yy");

        case DateWeekdayMonthDay:
            // NOTE: here YMD and MDY are the same, so we pass tr() a context string
            //   in case they need to differ in some translation

            if (order == DateFormatDMY)
                //: weekday, month, and day where user chose DMY order (e.g. Monday, 31 January)
                //: Translate the format delimiters, not order
                //: dddd = full day of week, MMMM = full month, d = day of month (QDateTime format string)
                return tr("dddd, d MMMM");
            if (order == DateFormatYMD)
                //: weekday, month, and day where user chose YMD order (e.g. Monday, January 31)
                //: Translate the format delimiters, not order
                //: dddd = full day of week, MMMM = full month, d = day of month (QDateTime format string)
                return tr("dddd, MMMM d", "YMD");
            //: weekday, month, and day where user chose MDY order (e.g. Monday, January 31)
            //: Translate the format delimiters, not order
            //: dddd = full day of week, MMMM = full month, d = day of month (QDateTime format string)
            return tr("dddd, MMMM d", "MDY");

        case DateWeekdayDayShort:
            // NOTE: here all cases are the same, so we pass tr() a context string
            //   in case they need to differ in some translation

            if (order == DateFormatDMY)
                //: weekday and day where user chose DMY order (e.g. Mon 31)
                //: Translate the format delimiters, not order
                //: ddd = short day of week, d = day of month (QDateTime format string)
                return tr("ddd d", "DMY");
            if (order == DateFormatYMD)
                //: weekday and day where user chose YMD order (e.g. Mon 31)
                //: Translate the format delimiters, not order
                //: ddd = short day of week, d = day of month (QDateTime format string)
                return tr("ddd d", "YMD");
            //: weekday and day where user chose MDY order (e.g. Mon 31)
            //: Translate the format delimiters, not order
            //: ddd = short day of week, d = day of month (QDateTime format string)
            return tr("ddd d", "MDY");

        case DateMonthDay:
            // NOTE: here YMD and MDY are the same, so we pass tr() a context string
            //   in case they need to differ in some translation

            if (order == DateFormatDMY)
                //: month and day where user chose DMY order (e.g. 31 January)
                //: Translate the format delimiters, not order
                //: MMMM = full month, d = day of month (QDateTime format string)
                return tr("d MMMM");
            if (order == DateFormatYMD)
                //: month and day where user chose YMD order (e.g. January 31)
                //: Translate the format delimiters, not order
                //: MMMM = full month, d = day of month (QDateTime format string)
                return tr("MMMM d", "YMD");
            //: month and day where user chose MDY order (e.g. January 31)
            //: Translate the format delimiters, not order
            //: MMMM = full month, d = day of month (QDateTime format string)
            return tr("MMMM d", "MDY");

        case DateMonthYear:
            // NOTE: here DMY and MDY are the same, so we pass tr() a context string
            //   in case they need to differ in some translation

            if (order == DateFormatDMY)
                //: month and year where user chose DMY order (e.g. January 2011)
                //: Translate the format delimiters, not order
                //: MMMM = full month, yyyy = full year (QDateTime format string)
                return tr("MMMM yyyy", "DMY");
            if (order == DateFormatYMD)
                //: month and year where user chose YMD order (e.g. 2011 January)
                //: Translate the format delimiters, not order
                //: MMMM = full month, yyyy = full year (QDateTime format string)
                return tr("yyyy MMMM");
            //: month and year where user chose MDY order (e.g. January 2011)
            //: Translate the format delimiters, not order
            //: MMMM = full month, yyyy = full year (QDateTime format string)
            return tr("MMMM yyyy", "MDY");

        case DateMonthYearShort:
            // NOTE: here DMY and MDY are the same, so we pass tr() a context string
            //   in case they need to differ in some translation

            if (order == DateFormatDMY)
                //: short month and year where user chose DMY order (e.g. Jan 2011)
                //: Translate the format delimiters, not order
                //: MMM = short month, yyyy = full year (QDateTime format string)
                return tr("MMM yyyy", "DMY");
            if (order == DateFormatYMD)
                //: short month and year where user chose YMD order (e.g. 2011 Jan)
                //: Translate the format delimiters, not order
                //: MMM = short month, yyyy = full year (QDateTime format string)
                return tr("yyyy MMM");
            //: short month and year where user chose MDY order (e.g. Jan 2011)
            //: Translate the format delimiters, not order
            //: MMM = short month, yyyy = full year (QDateTime format string)
            return tr("MMM yyyy", "MDY");

        case DateDay:
            //: day of month (e.g. 31, no leading zero)
            //: You probably won't translate this
            //: d = day of month (QDateTime format string)
            return tr("d");

        case DateWeekday:
            //: full day of week (e.g. Monday)
            //: You probably won't translate this
            //: dddd = full day of week (QDateTime format string)
            return tr("dddd");

        case DateWeekdayShort:
            //: short day of week (e.g. Mon)
            //: You probably won't translate this
            //: ddd = short day of week (QDateTime format string)
            return tr("ddd");

        case DateMonth:
            //: full month (e.g. January)
            //: You probably won't translate this
            //: MMMM = full month (QDateTime format string)
            return tr("MMMM");

        case DateMonthShort:
            //: short month (e.g. Jan)
            //: You probably won't translate this
            //: MMM = short month (QDateTime format string)
            return tr("MMM");

        case DateYear:
            //: full year  (e.g. 2011)
            //: You probably won't translate this
            //: yyyy = full year (QDateTime format string)
            return tr("yyyy");

        default:
            qWarning() << "meego::Locale: unexpected format string";
            return QString();
        }
    }


    void Locale::readLanguageConfItem()
    {
        QString lang = mLanguageConfItem.value().toString();

        if( lang.isEmpty() )
            lang = QString::fromLatin1( "en_US" );

        mLocale = lang;

        setLocale( mLocale );
    }


    void Locale::readDateFormatConfItem()
    {
        QString dateformat = mDateFormatConfItem.value().toString();

        if( dateformat == "dmy" )
            mDateFormat = DateFormatDMY;
        else if( dateformat == "mdy" )
            mDateFormat = DateFormatMDY;
        else if( dateformat == "ymd" )
            mDateFormat = DateFormatYMD;
        else
            mDateFormat = defaultDateFormat();
    }


    void Locale::readTimeFormatConfItem()
    {
        int timeformat = mTimeFormatConfItem.value().toInt();

        if( 12 == timeformat )
            mTimeFormat = TimeFormat12;
        else if( 24 == timeformat )
            mTimeFormat = TimeFormat24;
        else
            mTimeFormat = defaultTimeFormat();
    }


    void Locale::readDecimalPointConfItem()
    {
        QString dec = mDecimalPointConfItem.value().toString();
        if( !dec.isEmpty() )
            mDecimalPoint = dec;
        else
            mDecimalPoint = defaultDecimalPoint() ;
    }


    void Locale::readFirstWeekConfItem()
    {
        QString firstday = mFirstDayOfWeekConfItem.value().toString();

        if( !firstday.isEmpty() ) {
            mFirstDayOfWeek = defaultFirstDayOfWeek();
        } else if (1 == firstday.length()) {
            QChar ch = firstday.at(0);
            if (ch >= '1' && ch <= '7') {
                mFirstDayOfWeek = (DayOfWeek)(int)(ch.toAscii() - '0');
            }
        } else if( "invalid" == firstday ) {
            //TODO -> set to invalid?
            mFirstDayOfWeek = defaultFirstDayOfWeek();
        } else {
            mFirstDayOfWeek = defaultFirstDayOfWeek();
        }
    }


} //namespace meego
