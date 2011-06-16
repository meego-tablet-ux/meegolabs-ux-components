/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "meegolocale.h"
#include "meegolocale_p.h"
#include <QDate>
#include <QDateTime>
#include <QTime>

#include <QDebug>


namespace meego {

    Locale::Locale(QObject *parent): QObject(parent)
    {
        m_24hour = false;

        //: translator: set to 1 for default 24-hour time format (0 for 12-hour)
        QString time24 = tr("0", "timeformat");
        QChar ch = time24.at(0);
        if (ch == '1')
            m_24hour = true;
    }

    QString Locale::localDate(const QDate &date, int format) const
    {
        Q_D( const Locale);

        if (format <= DateBEGIN || format >= DateEND) {
            qWarning() << "Locale: invalid date format: " << format;
            return QString();
        }

        return date.toString( d->formatString(format) );
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

        if (format == TimeFull)
            format = m_24hour ? TimeFull24 : TimeFull12;

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
            return time.toString(m_locale.timeFormat(QLocale::LongFormat));

        case TimeQtShort:
            return time.toString(m_locale.timeFormat(QLocale::ShortFormat));

        default:
            qWarning() << "Locale: unhandled time format: " << format;
            return QString();
        }
    }

    QString Locale::localDateTime(const QDateTime &datetime, int format) const
    {
        Q_D( const Locale );

        if (format <= DateTimeBEGIN || format >= DateTimeEND) {
            qWarning() << "Locale: invalid datetime format: " << format;
            return QString();
        }

        return datetime.toString( d->formatString(format) );
    }

    QString Locale::currentDate(int format) const
    {
        Q_D( const Locale );
        return localDate(QDate::currentDate(), format);
    }

    QString Locale::currentTime(int format) const
    {
        Q_D( const Locale );
        return localTime(QTime::currentTime(), format);
    }

    QString Locale::currentDateTime(int format) const
    {
        Q_D( const Locale );
        return localDateTime(QDateTime::currentDateTime(), format);
    }

    QString Locale::numericDateOrder() const
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

    int Locale::firstDayOfWeek() const
    {
        Q_D(const Locale);

        return d->m_currenFirstDayOfWeek;

        //TODO: move that to private

        //: translator: identify the first day of the week in target culture
        //: 1 = Sunday, 2 = Monday, etc.
        QString day = tr("1", "firstday");
        QChar ch = day.at(0);
        int dayOfWeek = DaySunday;
        if (ch >= '1' && ch <= '7')
            dayOfWeek = ch.toAscii() - '0';
        return dayOfWeek;
    }

    QString Locale::decimalPoint() const
    {
        Q_D(const Locale);
        return d->m_locale.decimalPoint();
    }

    void Locale::setLanguageCode( const QString code )
    {
        Q_D(Locale);
        d->changeCurrentLanguage( code );
    }

    QString Locale::getCurrentLanguageCode() const
    {
        Q_D(const Locale);
        qWarning() << "not implemented";
        return d->m_currentLanguageCode;
    }
    QList<QString> Locale::getLanguageCodes() const
    {
        Q_D(const Locale);
        qWarning() << "not implemented";
        return d->m_locale.codes;
    }

    int Locale::firstDayOfWeek() const
    {
        Q_D(const Locale);
        qWarning() << "not implemented";
        return d->m_currentFirstDayOfWeek
    }
    void Locale::setFirstDayOfWeek( DayOfWeek dayofWeek )
    {
        Q_D(Locale);
        qWarning() << "not implemented";
        d->m_currentFirstDayOfWeek = dayofWeek;
    }

    int Locale::getDefaultFirstDayOfWeek() const
    {
        Q_D(const Locale);
        qWarning() << "not implemented";
        return d->m_defaultFirstDayOfWeek;
    }

    DateOrder Locale::getDateOrder() const
    {
        Q_D(const Locale);
        qWarning() << "not implemented";
        return d->m_currentDateOrder;
    }

    void Locale::setDateOrder( DateOrder dateOrder )
    {
        Q_D(Locale);
        qWarning() << "not implemented";
        d->m_currentDateOrder = dateOrder;
    }

    DateTimeFormat Locale::getDefaultDateFormat() const
    {
        Q_D(const Locale);
        qWarning() << "not implemented";
        return d->m_defaultDateFormat;
    }

    DateOrder Locale::getDefaultDateOrder()
    {
        Q_D(Locale);
        qWarning() << "not implemented";
        return d->m_defaultDateOrder;
    }

    DateTimeFormat Locale::getDefaultTimeFormat() const
    {
        Q_D(const Locale);
        qWarning() << "not implemented";
        return d->m_defaultTimeFormat;
    }

    DateTimeFormat Locale::getDefaultDateTimeFormat() const
    {
        Q_D(const Locale);
        Warning() << "not implemented";
        return d->m_defaultDateTimeFormat;
    }

    HourFormat Locale::getHourFormat() const
    {
        Q_D(const Locale);
        qWarning() << "not implemented";
        return d->m_currentHourFormat;
    }

    void Locale::setHourFormat( HourFormat hourFormat )
    {
        Q_D(Locale);
        qWarning() << "not implemented";
        d->m_currentHourFormat = hourFormat;
    }

    //
    // PRIVATE
    //
    void LocalePrivate::readConfig() {
        qWarning() << "not implemented";
    }
    void LocalePrivate::resetToDefault() {
        qWarning() << "not implemented";
    }

} //namespace meego
