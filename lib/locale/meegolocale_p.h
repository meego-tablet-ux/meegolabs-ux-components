#ifndef MEEGOLOCALE_P_H
#define MEEGOLOCALE_P_H

#include <QObject>
#include <QLocale>
#include <QtCore/qglobal.h>

#include "meegolocale.h"

namespace meego {

    class LocalePrivate : QObject {
        Q_OBJECT

    public:
        LocalePrivate( Locale* parent) : QObject( parent ), q_ptr( parent )
        {
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
}
#endif // MEEGOLOCALE_P_H
