#ifndef MEEGOLOCALE_P_H
#define MEEGOLOCALE_P_H

#include "meegolocale.h"

namespace meego {

    class LocalePrivate {

    public:
        LocalePrivate( Locale* parent) : q_ptr( parent)
        {
            m_currentLanguage = QString();
            m_currentLanguageCode = QString();
            m_currentRegionCode = QString();

            m_currentDateOrder = MDY;
            m_defaultDateOrder = MDY;
            m_currentHourFormat = Hrs24;
            m_defaultHourFormat = Hrs24;
            m_currentDefaultDateTimeFormat = DateTimeFull;
            m_defaultDateTimeFormat = DateTimeFull;
            m_currentDefaultTimeFormat = TimeFull;
            m_defaultTimeFormat = TimeTime;
            m_currentDefaultDateFormat = DateFull;
            m_defaultDateFormat = DateFull;
            m_defaultFirstDayOfWeek = DaySunday;
            m_currentFirstDayOfWeek = DaySunday;
        }

        void readConfig();
        void resetToDefault();
        void changeLanguage(const QString languageString);

        DateOrder m_currentDateOrder;
        DateOrder m_defaultDateOrder;

        HourFormat m_currentHourFormat;
        HourFormat m_defaultHourFormat;

        DateTimeFormat m_currentDefaultDateTimeFormat;
        DateTimeFormat m_defaultDateTimeFormat;

        DateTimeFormat m_currentDefaultTimeFormat;
        DateTimeFormat m_defaultTimeFormat;

        DateTimeFormat m_currentDefaultDateFormat;
        DateTimeFormat m_defaultDateFormat;

        DayOfWeek m_defaultFirstDayOfWeek;
        DayOfWeek m_currentFirstDayOfWeek;

        QString m_currentLanguage;
        QString m_currentLanguageCode;
        QString m_currentRegionCode;
        QLocale m_locale;

        Locale* const q_ptr;
        Q_DECLARE_PUBLIC( Locale );

    };
}
#endif // MEEGOLOCALE_P_H
