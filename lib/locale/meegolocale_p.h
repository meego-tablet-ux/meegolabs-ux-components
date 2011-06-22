/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGO_LOCALE_P_H
#define MEEGO_LOCALE_P_H

#include "meegolocale.h"
#include <QObject>
#include <QLocale>
#include <QtCore/qglobal.h>

#include <QDebug>


namespace meego
{

    class LocalePrivate : QObject
    {
        Q_OBJECT;

    public:
        
        LocalePrivate( Locale* parent)
            : QObject( parent ),
              q_ptr( parent )
        {
            qDebug() << "Starting LocalePrivate...";
            m_currentLanguage = QString();
            m_currentLanguageCode = QString();
            m_currentRegionCode = QString();

            m_currentDateOrder = Locale::MDY;
            m_defaultDateOrder = Locale::MDY;
            m_currentHourFormat = Locale::Hrs24;
            m_defaultHourFormat = Locale::Hrs24;
            m_currentDateTimeFormat = Locale::DateTimeQtLong;
            m_defaultDateTimeFormat = Locale::DateTimeQtLong;
            m_currentTimeFormat = Locale::TimeFull;
            m_defaultTimeFormat = Locale::TimeFull;
            m_currentDateFormat = Locale::DateFull;
            m_defaultDateFormat = Locale::DateFull;
            m_defaultFirstDayOfWeek = Locale::DaySunday;
            m_currentFirstDayOfWeek = Locale::DaySunday;
        }

        void readConfig();
        void resetToDefault();
        void changeLanguage(const QString languageString);

        Locale::DateOrder m_currentDateOrder;
        Locale::DateOrder m_defaultDateOrder;

        Locale::HourFormat m_currentHourFormat;
        Locale::HourFormat m_defaultHourFormat;

        Locale::DateTimeFormat m_currentDateTimeFormat;
        Locale::DateTimeFormat m_defaultDateTimeFormat;

        Locale::DateTimeFormat m_currentTimeFormat;
        Locale::DateTimeFormat m_defaultTimeFormat;

        Locale::DateTimeFormat m_currentDateFormat;
        Locale::DateTimeFormat m_defaultDateFormat;

        Locale::DayOfWeek m_defaultFirstDayOfWeek;
        Locale::DayOfWeek m_currentFirstDayOfWeek;

        QString m_currentLanguage;
        QString m_currentLanguageCode;
        QString m_currentRegionCode;
        QLocale m_locale;

        Locale* const q_ptr;
        Q_DECLARE_PUBLIC( Locale );

    };
    
} //namespace meego

#endif // MEEGO_LOCALE_P_H
