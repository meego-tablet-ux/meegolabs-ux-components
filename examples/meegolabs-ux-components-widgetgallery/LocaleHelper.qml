/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

/* This file contains relativy empty pages and is meant to demonstrate the
   book/page concept */

import Qt 4.7
import MeeGo.Ux.Components.Common 0.1
import MeeGo.Labs.Components 0.1 as Labs

AppPage {
    id: widgetPage

    pageTitle: qsTr("Locale Helper")

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    Flickable {
        id: flow
        anchors.fill: parent
        contentHeight: Math.max(c0.height, c1.height, c2.height)

        Row {
            Column {
                id: c0
                Text { text: "<b>General</b>" ; font.pixelSize: 18; width: 300; height: 40 }
                Text { text: "<b>locale</b>: "   + localehelper.locale; width: 300; height: 25  }

                Text { text: "<b>defaultDecimalPoint</b>: "   + localehelper.defaultDecimalPoint; width: 300; height: 25  }
                Text { text: "<b>decimalPoint</b>: "   + localehelper.decimalPoint; width: 300; height: 25  }
                Text { text: "<b>defaultFirstDayOfWeek</b>: "   + localehelper.defaultFirstDayOfWeek; width: 300; height: 25  }
                Text { text: "<b>firstDayOfWeek</b>: "   + localehelper.firstDayOfWeek; width: 300; height: 25  }
                Text { text: "<b>defaultDateFormat</b>: "   + localehelper.defaultDateFormat; width: 300; height: 25  }
                Text { text: "<b>dateFormat</b>: "   + localehelper.dateFormat; width: 300; height: 25  }
                Text { text: "<b>defaultTimeFormat</b>: "   + localehelper.defaultTimeFormat; width: 300; height: 25  }
                Text { text: "<b>TimeFormat</b>: "   + localehelper.timeFormat; width: 300; height: 25  }

                Text { text: "<b>installedLocales</b>:"; width: 300; height: 25  }
                Column {
                    Repeater {
                        model: Labs.LocaleListModel {}
                        delegate:  Text {
                            text: "   - " + displayLocale + " [" + locale + "]"
                            width:300
                            height: 25
                        }
                    }
                }
            }
            Column {
                id: c1
                Text { text: "<b>Date Format</b>" ; font.pixelSize: 18; width: 300; height: 25 }

                DropDown {
                    width:  250
                    height: 40

                    title: "Date Format"
                    showTitleInMenu: true
                    model:  [ "Year, Month, Day",
                              "Day, Month, Year",
                              "Month, Day, Year" ]
                    payload: [ Labs.LocaleHelper.DateFormatYMD,
                               Labs.LocaleHelper.DateFormatDMY,
                               Labs.LocaleHelper.DateFormatMDY ]
                    selectedIndex: localehelper.dateFormat
                    onTriggered:  {
                        localehelper.dateFormat = payload[index]
                    }
                }

                LocaleHelperDate { label: "DateFullLong"; format: Labs.LocaleHelper.DateFullLong }
                LocaleHelperDate { label: "DateFull"; format: Labs.LocaleHelper.DateFull }
                LocaleHelperDate { label: "DateFullShort"; format: Labs.LocaleHelper.DateFullShort }
                LocaleHelperDate { label: "DateFullNum"; format: Labs.LocaleHelper.DateFullNum }
                LocaleHelperDate { label: "DateFullNumShort"; format: Labs.LocaleHelper.DateFullNumShort }
                LocaleHelperDate { label: "DateWeekdayMonthDay"; format: Labs.LocaleHelper.DateWeekdayMonthDay }
                LocaleHelperDate { label: "DateWeekdayDayShort"; format: Labs.LocaleHelper.DateWeekdayDayShort }
                LocaleHelperDate { label: "DateMonthDay"; format: Labs.LocaleHelper.DateMonthDay }
                LocaleHelperDate { label: "DateMonthYear"; format: Labs.LocaleHelper.DateMonthYear }
                LocaleHelperDate { label: "DateMonthYearShort"; format: Labs.LocaleHelper.DateMonthYearShort }
                LocaleHelperDate { label: "DateDay"; format: Labs.LocaleHelper.DateDay }
                LocaleHelperDate { label: "DateWeekday"; format: Labs.LocaleHelper.DateWeekday }
                LocaleHelperDate { label: "DateWeekdayShort"; format: Labs.LocaleHelper.DateWeekdayShort }
                LocaleHelperDate { label: "DateMonth"; format: Labs.LocaleHelper.DateMonth }
                LocaleHelperDate { label: "DateMonthShort"; format: Labs.LocaleHelper.DateMonthShort }
                LocaleHelperDate { label: "DateYear"; format: Labs.LocaleHelper.DateYear }
            }
            Column {
                id: c2
                Text { text: "<b>Time Format</b>" ; font.pixelSize: 18; width: 300; height: 25 }

                DropDown {
                    width:  250
                    height: 40

                    title: "Time Format"
                    showTitleInMenu: true
                    model:  [ "12 Hour",
                              "24 Hour"]
                    payload: [ Labs.LocaleHelper.TimeFormat12,
                               Labs.LocaleHelper.TimeFormat24 ]
                    selectedIndex: localehelper.timeformat
                    onTriggered:  {
                        localehelper.timeFormat = payload[index]
                    }
                }

                LocaleHelperTime { label: "TimeFull";   format: Labs.LocaleHelper.TimeFull }
                LocaleHelperTime { label: "TimeFull12"; format: Labs.LocaleHelper.TimeFull12 }
                LocaleHelperTime { label: "TimeFull24"; format: Labs.LocaleHelper.TimeFull24 }
            }
        }
    }


    Labs.LocaleHelper {
        id: localehelper
    }

}
