#ifndef MEEGOLOCALE_P_H
#define MEEGOLOCALE_P_H

#include "meegolocale.h"

namespace meego {

    class LocalePrivate {

    public:
        LocalePrivate( Locale* parent) : q_ptr( parent) {}

        void readConfig();
        void resetToDefault();
        void changeLanguage(const QString languageString);

        QString formatString(int format) const;

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
        QLocale m_locale;

        Locale* const q_ptr;
        Q_DECLARE_PUBLIC( Locale );

    };
}
#endif // MEEGOLOCALE_P_H
