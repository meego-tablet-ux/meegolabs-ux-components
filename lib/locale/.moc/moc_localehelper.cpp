/****************************************************************************
** Meta object code from reading C++ file 'localehelper.h'
**
** Created: Tue Jun 14 18:54:15 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../localehelper.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'localehelper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LocaleHelper[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       2,   59, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      34,   22,   14,   13, 0x02,
      67,   55,   14,   13, 0x02,
     104,   88,   14,   13, 0x02,
     140,  133,   14,   13, 0x02,
     157,  133,   14,   13, 0x02,
     174,  133,   14,   13, 0x02,
     195,   13,   14,   13, 0x02,
     218,   13,  214,   13, 0x02,
     235,   13,   14,   13, 0x02,

 // enums: name, flags, count, data
     250, 0x0,   33,   67,
     265, 0x0,    7,  133,

 // enum data: key, value
     275, uint(LocaleHelper::DateBEGIN),
     285, uint(LocaleHelper::DateFullLong),
     298, uint(LocaleHelper::DateFull),
     307, uint(LocaleHelper::DateFullShort),
     321, uint(LocaleHelper::DateFullNum),
     333, uint(LocaleHelper::DateFullNumShort),
     350, uint(LocaleHelper::DateWeekdayMonthDay),
     370, uint(LocaleHelper::DateWeekdayDayShort),
     390, uint(LocaleHelper::DateMonthDay),
     403, uint(LocaleHelper::DateMonthYear),
     417, uint(LocaleHelper::DateMonthYearShort),
     436, uint(LocaleHelper::DateDay),
     444, uint(LocaleHelper::DateWeekday),
     456, uint(LocaleHelper::DateWeekdayShort),
     473, uint(LocaleHelper::DateMonth),
     483, uint(LocaleHelper::DateMonthShort),
     498, uint(LocaleHelper::DateYear),
     507, uint(LocaleHelper::DateWeekYear),
     520, uint(LocaleHelper::DateWeekYearShort),
     538, uint(LocaleHelper::DateQtLong),
     549, uint(LocaleHelper::DateQtShort),
     561, uint(LocaleHelper::DateEND),
     569, uint(LocaleHelper::TimeBEGIN),
     579, uint(LocaleHelper::TimeFull),
     588, uint(LocaleHelper::TimeFull12),
     599, uint(LocaleHelper::TimeFull24),
     610, uint(LocaleHelper::TimeQtLong),
     621, uint(LocaleHelper::TimeQtShort),
     633, uint(LocaleHelper::TimeEND),
     641, uint(LocaleHelper::DateTimeBEGIN),
     655, uint(LocaleHelper::DateTimeQtLong),
     670, uint(LocaleHelper::DateTimeQtShort),
     686, uint(LocaleHelper::DateTimeEND),
     698, uint(LocaleHelper::DaySunday),
     708, uint(LocaleHelper::DayMonday),
     718, uint(LocaleHelper::DayTuesday),
     729, uint(LocaleHelper::DayWednesday),
     742, uint(LocaleHelper::DayThursday),
     754, uint(LocaleHelper::DayFriday),
     764, uint(LocaleHelper::DaySaturday),

       0        // eod
};

static const char qt_meta_stringdata_LocaleHelper[] = {
    "LocaleHelper\0\0QString\0date,format\0"
    "localDate(QDate,int)\0time,format\0"
    "localTime(QTime,int)\0datetime,format\0"
    "localDateTime(QDateTime,int)\0format\0"
    "currentDate(int)\0currentTime(int)\0"
    "currentDateTime(int)\0numericDateOrder()\0"
    "int\0firstDayOfWeek()\0decimalPoint()\0"
    "DateTimeFormat\0DayOfWeek\0DateBEGIN\0"
    "DateFullLong\0DateFull\0DateFullShort\0"
    "DateFullNum\0DateFullNumShort\0"
    "DateWeekdayMonthDay\0DateWeekdayDayShort\0"
    "DateMonthDay\0DateMonthYear\0"
    "DateMonthYearShort\0DateDay\0DateWeekday\0"
    "DateWeekdayShort\0DateMonth\0DateMonthShort\0"
    "DateYear\0DateWeekYear\0DateWeekYearShort\0"
    "DateQtLong\0DateQtShort\0DateEND\0TimeBEGIN\0"
    "TimeFull\0TimeFull12\0TimeFull24\0"
    "TimeQtLong\0TimeQtShort\0TimeEND\0"
    "DateTimeBEGIN\0DateTimeQtLong\0"
    "DateTimeQtShort\0DateTimeEND\0DaySunday\0"
    "DayMonday\0DayTuesday\0DayWednesday\0"
    "DayThursday\0DayFriday\0DaySaturday\0"
};

const QMetaObject LocaleHelper::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LocaleHelper,
      qt_meta_data_LocaleHelper, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LocaleHelper::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LocaleHelper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LocaleHelper::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LocaleHelper))
        return static_cast<void*>(const_cast< LocaleHelper*>(this));
    return QObject::qt_metacast(_clname);
}

int LocaleHelper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { QString _r = localDate((*reinterpret_cast< const QDate(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 1: { QString _r = localTime((*reinterpret_cast< const QTime(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 2: { QString _r = localDateTime((*reinterpret_cast< const QDateTime(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 3: { QString _r = currentDate((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 4: { QString _r = currentTime((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: { QString _r = currentDateTime((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 6: { QString _r = numericDateOrder();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 7: { int _r = firstDayOfWeek();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 8: { QString _r = decimalPoint();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
