#include <QtCore/QString>
#include <QtTest/QtTest>

#include <meegolocale.h>

class MeegoLocaleTest : public QObject
{
    Q_OBJECT

public:
    MeegoLocaleTest();

private Q_SLOTS:
    void tst_createLib();
    void tst_setLanguage();
    void tst_setLanguageNegative();
    void tst_setDecimalPoint();
    void tst_setDecimalPointNegative();
    void tst_setDateFormat();
    void tst_setDateFormatNegative();
    void tst_setTimeFormat();
    void tst_setTimeFormatNegative();
    void tst_setFirstDayOfWeek();
    void tst_setFirstDayOfWeekNegative();
};

MeegoLocaleTest::MeegoLocaleTest()
{
}

void MeegoLocaleTest::tst_createLib()
{
    meego::Locale* locale = new meego::Locale();
    QVERIFY( locale != 0);
    delete locale;
}

void MeegoLocaleTest::tst_setLanguage()
{
    meego::Locale* locale = new meego::Locale();

    QString lang1 = "en_US";
    QString lang2 = "de_DE";

    locale->setLocale( lang1 );
    QVERIFY( lang1 == locale->locale() );

    locale->setLocale( lang2 );
    QVERIFY( lang2 == locale->locale() );

    delete locale;
}

void MeegoLocaleTest::tst_setLanguageNegative()
{
    meego::Locale* locale = new meego::Locale();

    QString lang1 = "xy_zg";
    QString lang2 = "12_12";

    locale->setLocale( lang1 );
    QVERIFY( lang1 != locale->locale() );

    locale->setLocale( lang2 );
    QVERIFY( lang2 != locale->locale() );

    delete locale;

}


void MeegoLocaleTest::tst_setDecimalPoint()
{
    meego::Locale* locale = new meego::Locale();

    QString dec1(".");
    QString dec2(",");

    locale->setDecimalPoint( dec1 );
    QVERIFY( locale->decimalPoint() == dec1  );

    locale->setDecimalPoint( dec2 );
    QVERIFY( locale->decimalPoint() == dec2  );

    delete locale;
}


void MeegoLocaleTest::tst_setDecimalPointNegative()
{
    meego::Locale* locale = new meego::Locale();

    QString defaultDec = locale->defaultDecimalPoint();
    QString dec1(".aa");
    QString dec2("12f,");

    locale->setDecimalPoint( dec1 );
    QVERIFY( locale->decimalPoint() == defaultDec );

    locale->setDecimalPoint( dec2 );
    QVERIFY( locale->decimalPoint() == defaultDec );

    delete locale;
}


void MeegoLocaleTest::tst_setDateFormat()
{
    meego::Locale* locale = new meego::Locale();

    meego::Locale::DateFormat format1 = meego::Locale::DateFormatDMY;
    meego::Locale::DateFormat format2 = meego::Locale::DateFormatMDY;
    meego::Locale::DateFormat format3 = meego::Locale::DateFormatYMD;
    meego::Locale::DateFormat format4 = meego::Locale::DateFormatInvalid;

    locale->setDateFormat( format1 );
    QVERIFY( format1 == locale->dateFormat() );

    locale->setDateFormat( format2 );
    QVERIFY( format1 == locale->dateFormat() );

    locale->setDateFormat( format3 );
    QVERIFY( format1 == locale->dateFormat() );

    locale->setDateFormat( format4 );
    QVERIFY( format1 == locale->dateFormat() );

    delete locale;
}


void MeegoLocaleTest::tst_setDateFormatNegative()
{
    meego::Locale* locale = new meego::Locale();

    meego::Locale::DateFormat format1 = meego::Locale::DateFormatDMY;
    meego::Locale::DateFormat format2 = meego::Locale::DateFormatMDY;
    meego::Locale::DateFormat format3 = meego::Locale::DateFormatYMD;
    meego::Locale::DateFormat format4 = meego::Locale::DateFormatInvalid;

    locale->setDateFormat( meego::Locale::DateFormatInvalid );
    QVERIFY( format1 != locale->dateFormat() );

    locale->setDateFormat( meego::Locale::DateFormatInvalid );
    QVERIFY( format2 != locale->dateFormat() );

    locale->setDateFormat( meego::Locale::DateFormatInvalid );
    QVERIFY( format3 != locale->dateFormat() );

    locale->setDateFormat( meego::Locale::DateFormatInvalid );
    QVERIFY( format4 == locale->dateFormat() );

    delete locale;

}


void MeegoLocaleTest::tst_setTimeFormat()
{
    meego::Locale* locale = new meego::Locale();

    meego::Locale::TimeFormat format1 = meego::Locale::TimeFormat12;
    meego::Locale::TimeFormat format2 = meego::Locale::TimeFormat24;
    meego::Locale::TimeFormat format3 = meego::Locale::TimeFormatInvalid;

    locale->setTimeFormat( format1 );
    QVERIFY( locale->timeFormat() == format1 );
    locale->setTimeFormat( format2 );
    QVERIFY( locale->timeFormat() == format2 );
    locale->setTimeFormat( format3 );
    QVERIFY( locale->timeFormat() == format3 );
    locale->resetTimeFormat();
    QVERIFY( locale->defaultTimeFormat() == locale->timeFormat() );
    delete locale;
}


void MeegoLocaleTest::tst_setTimeFormatNegative()
{
    meego::Locale* locale = new meego::Locale();

    meego::Locale::TimeFormat format1 = meego::Locale::TimeFormat12;
    meego::Locale::TimeFormat format2 = meego::Locale::TimeFormat24;
    meego::Locale::TimeFormat format3 = meego::Locale::TimeFormatInvalid;

    locale->setTimeFormat( format3 );
    QVERIFY( locale->timeFormat() != format1 );
    locale->setTimeFormat( format3 );
    QVERIFY( locale->timeFormat() != format2 );
    locale->setTimeFormat( format3 );
    QVERIFY( locale->timeFormat() == format3 );
    locale->resetTimeFormat();
    QVERIFY( locale->defaultTimeFormat() == locale->timeFormat() );

    delete locale;

}

void MeegoLocaleTest::tst_setFirstDayOfWeek()
{
    meego::Locale* locale = new meego::Locale();

    meego::Locale::DayOfWeek def = locale->defaultFirstDayOfWeek();
    meego::Locale::DayOfWeek mon = meego::Locale::DayMonday;
    meego::Locale::DayOfWeek tue = meego::Locale::DayTuesday;
    meego::Locale::DayOfWeek wen = meego::Locale::DayWednesday;
    meego::Locale::DayOfWeek thu = meego::Locale::DayThursday;
    meego::Locale::DayOfWeek fri = meego::Locale::DayFriday;
    meego::Locale::DayOfWeek sat = meego::Locale::DaySaturday;
    meego::Locale::DayOfWeek sun = meego::Locale::DaySunday;
    meego::Locale::DayOfWeek inv = meego::Locale::DayInvalid;

    locale->setFirstDayOfWeek( mon );
    QVERIFY( locale->firstDayOfWeek() == mon );
    locale->setFirstDayOfWeek( tue );
    QVERIFY( locale->firstDayOfWeek() == tue );
    locale->setFirstDayOfWeek( wen );
    QVERIFY( locale->firstDayOfWeek() == wen );
    locale->setFirstDayOfWeek( thu );
    QVERIFY( locale->firstDayOfWeek() == thu );
    locale->setFirstDayOfWeek( fri );
    QVERIFY( locale->firstDayOfWeek() == fri );
    locale->setFirstDayOfWeek( sat );
    QVERIFY( locale->firstDayOfWeek() == sat );
    locale->setFirstDayOfWeek( sun );
    QVERIFY( locale->firstDayOfWeek() == sun );

    locale->setFirstDayOfWeek( inv );
    QVERIFY( locale->firstDayOfWeek() == inv );

    locale->setFirstDayOfWeek( wen );
    QVERIFY( locale->firstDayOfWeek() == wen );

    locale->resetFirstDayOfWeek();
    QVERIFY( locale->firstDayOfWeek() == def );

    delete locale;

}


void MeegoLocaleTest::tst_setFirstDayOfWeekNegative()
{
    meego::Locale* locale = new meego::Locale();

    delete locale;

}

QTEST_APPLESS_MAIN(MeegoLocaleTest);

#include "tst_meegolocale.moc"



