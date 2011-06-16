/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QDate>
#include <QDateTime>
#include <QTime>
#include <QDebug>
#include <QLocale>

#include "meegolocale.h"
#include "meegolocale_p.h"

namespace meego {

    Locale::Locale(QObject *parent):
            QObject(parent),
            d_ptr( new LocalePrivate( this ) )
    {
        Q_D(Locale);
        d->readConfig();
    }

    QString Locale::localDate(const QDate &date, int format) const
    {
        if (format <= DateBEGIN || format >= DateEND) {
            qWarning() << "Locale: invalid date format: " << format;
            return QString();
        }

        return date.toString( formatString(format) );
    }

    QString Locale::localTime(const QTime &time, int format) const
    {
        Q_D( const Locale);

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
            if( d->m_currentHourFormat == Hrs12 )
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

        case TimeQtLong:
            return time.toString( d->m_locale.timeFormat(QLocale::LongFormat));

        case TimeQtShort:
            return time.toString( d->m_locale.timeFormat(QLocale::ShortFormat));

        default:
            qWarning() << "Locale: unhandled time format: " << format;
            return QString();
        }
    }

    QString Locale::localDateTime(const QDateTime &datetime, int format) const
    {
        if (format <= DateTimeBEGIN || format >= DateTimeEND) {
            qWarning() << "Locale: invalid datetime format: " << format;
            return QString();
        }

        return datetime.toString( formatString(format) );
    }

    QString Locale::currentDate(int format) const
    {
        return localDate(QDate::currentDate(), format);
    }

    QString Locale::currentTime(int format) const
    {
        return localTime(QTime::currentTime(), format);
    }

    QString Locale::currentDateTime(int format) const
    {
        return localDateTime(QDateTime::currentDateTime(), format);
    }

    QString Locale::numericDateOrder() const
    {
        Q_D( const Locale );

        if( DMY == d->m_currentDateOrder )
            return QString::fromLatin1("dmy");
        else if( YMD == d->m_currentDateOrder )
            return QString::fromLatin1("ymd");
        else if( MDY )
            return QString::fromLatin1("mdy");

        return QString::fromLatin1("dmy");
    }

    QString Locale::decimalPoint() const
    {
        Q_D(const Locale);
        return d->m_locale.decimalPoint();
    }

    void Locale::setLanguageCode( const QString code )
    {
        Q_D(Locale);
        d->changeLanguage( code );
    }

    QString Locale::getCurrentLanguageCode() const
    {
        Q_D(const Locale);
        return d->m_currentLanguageCode;
    }

    QList<QString> Locale::getLanguageCodes() const
    {
        return QList<QString>(); //TODO where comes the codes from?
    }

    Locale::DayOfWeek Locale::firstDayOfWeek() const
    {
        Q_D(const Locale);
        return d->m_currentFirstDayOfWeek;
    }
    void Locale::setFirstDayOfWeek( DayOfWeek dayofWeek )
    {
        Q_D(Locale);
        d->m_currentFirstDayOfWeek = dayofWeek;
    }

    Locale::DayOfWeek Locale::getDefaultFirstDayOfWeek() const
    {
        Q_D(const Locale);
        return d->m_defaultFirstDayOfWeek;
    }

    Locale::DateOrder Locale::getDateOrder() const
    {
        Q_D(const Locale);
        return d->m_currentDateOrder;
    }

    void Locale::setDateOrder( Locale::DateOrder dateOrder )
    {
        Q_D(Locale);
        qWarning() << "DateOrder is not yet implemented";
        d->m_currentDateOrder = dateOrder;
    }

   Locale:: DateTimeFormat Locale::getDefaultDateFormat() const
    {
        Q_D(const Locale);
        return d->m_defaultDateFormat;
    }

    Locale::DateOrder Locale::getDefaultDateOrder() const
    {
        Q_D(const Locale);
        return d->m_defaultDateOrder;
    }

    Locale::DateTimeFormat Locale::getDefaultTimeFormat() const
    {
        Q_D(const Locale);
        return d->m_defaultTimeFormat;
    }

    Locale::DateTimeFormat Locale::getDefaultDateTimeFormat() const
    {
        Q_D(const Locale);
        return d->m_defaultDateTimeFormat;
    }

    Locale::HourFormat Locale::getHourFormat() const
    {
        Q_D(const Locale);
        return d->m_currentHourFormat;
    }

    void Locale::setHourFormat( Locale::HourFormat hourFormat )
    {
        Q_D(Locale);
        d->m_currentHourFormat = hourFormat;
    }

    QString Locale::formatString(int format) const
    {
        Q_D(const Locale);

        DateTimeFormat myFormat = (DateTimeFormat)format;

        switch (myFormat) {
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
            return d->m_locale.dateFormat(QLocale::LongFormat);

        case DateQtShort:
            // e.g. 1/31/11
            return d->m_locale.dateFormat(QLocale::ShortFormat);

        case DateTimeQtLong:
            return d->m_locale.dateTimeFormat(QLocale::LongFormat);

        case DateTimeQtShort:
            return d->m_locale.dateTimeFormat(QLocale::ShortFormat);

        default:
            qWarning() << "Locale: unexpected format string";
            return QString();
        }
    }

    //
    // PRIVATE
    //
    void LocalePrivate::readConfig() {

        //: read DateOrder
        //: translator: this order will determine order of date picker widgets
        //: default order for numeric date (m = month, d = date, y = year)
        //: the string should contain these exact three characters in some order
        QString dateOrder = tr("mdy", "dateorder");

        if (dateOrder.length() == 3 && dateOrder.indexOf("m") != -1 &&
            dateOrder.indexOf("d") != -1 &&  dateOrder.indexOf("y") != -1) {

            if( dateOrder.indexOf("d") == 1 ) {
                m_defaultDateOrder = Locale::DMY;
                m_currentDateOrder = Locale::DMY;
            } else if ( dateOrder.indexOf("m") == 1 ) {
                m_defaultDateOrder = Locale::MDY;
                m_currentDateOrder = Locale::MDY;
            } else if ( dateOrder.indexOf("y") == 1 ) {
                m_defaultDateOrder = Locale::YMD;
                m_currentDateOrder = Locale::YMD;
            }
        }

        //: read 24 hrs flag DateOrder
        bool m_24hour = false;

        //: translator: set to 1 for default 24-hour time format (0 for 12-hour)
        QString time24 = tr("0", "timeformat");
        QChar ch = time24.at(0);
        if (ch == '1')
            m_24hour = true;

        if( m_24hour ) {
            m_currentHourFormat = Locale::Hrs24;
            m_defaultHourFormat = Locale::Hrs24;
        } else {
            m_currentHourFormat = Locale::Hrs12;
            m_defaultHourFormat = Locale::Hrs12;
        }

        //: read DateOfWeek
        Locale::DayOfWeek dayOfWeek = Locale::DaySunday;
        int dow = 0;
        QString day = tr("1", "firstday");
        ch = day.at(0);
        if (ch >= '1' && ch <= '7') {
            dow = ch.toAscii() - '0';
        }

        dayOfWeek = (Locale::DayOfWeek)dow;

        m_defaultFirstDayOfWeek = dayOfWeek;
        m_currentFirstDayOfWeek = dayOfWeek;

    }
    void LocalePrivate::resetToDefault()
    {
        qWarning() << "not implemented";
    }
    void LocalePrivate::changeLanguage(const QString languageString)
    {
        qWarning() << "not implemented";
    }

} //namespace meego
