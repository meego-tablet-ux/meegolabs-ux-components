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

    Labs.LocaleHelper {
        id: localehelper

        onLocaleChanged: {
            localeSignals.text += "locale changed to " + localehelper.locale + "<br>"
        }
        onDateFormatChanged: {
            localeSignals.text += "date format changed to " + localehelper.dateFormat + "<br>"
        }
        onTimeFormatChanged: {
            localeSignals.text += "time format changed to " + localehelper.timeFormat + "<br>"
        }
        onFirstDayOfWeekChanged: {
            localeSignals.text += "first day of week changed to " + localehelper.firstDayOfWeek + "<br>"
        }
        onCountryChanged: {
            localeSignals.text += "country changed to " + localehelper.country + "<br>"
        }
        
    }

    Labs.LocaleListModel {
        id: localeModel
    } 

    Flickable {
        id: flow
        anchors.fill: parent
        contentHeight: Math.max(c0.height, c1.height, c2.height)

        Row {
            Column {
                id: c0
                Text { text: "<b>General</b>" ; font.pixelSize: 18; width: 300; height: 25 }
                DropDown {
                    height: 40

                    title: "Locale"
                    showTitleInMenu: true
                    model: localeModel.displayLocales()
                    payload: localeModel.locales()
                    selectedIndex: localeModel.indexOf(localehelper.locale)
                    onTriggered:  {
                        localehelper.locale = payload[index]
                    }
                }
                Text { 
                        text: "<b>defaultDecimalPoint</b>: "   + localehelper.defaultDecimalPoint; height: 25
                }
                Text { 
                        text: "<b>decimalPoint</b>: "   + localehelper.decimalPoint; height: 25
                }
                Text { 
                    text: "<b>defaultFirstDayOfWeek</b>: "   + localehelper.defaultFirstDayOfWeek; height: 25  
                }
                Text { 
                    text: "<b>firstDayOfWeek</b>: "   + localehelper.firstDayOfWeek; height: 25  
                }
                Text { 
                    text: "<b>defaultDateFormat</b>: "   + localehelper.defaultDateFormat; height: 25  
                }
                Text { 
                    text: "<b>dateFormat</b>: "   + localehelper.dateFormat; height: 25  
                }
                Text { 
                    text: "<b>defaultTimeFormat</b>: "   + localehelper.defaultTimeFormat; height: 25  
                }
                Text { 
                    text: "<b>TimeFormat</b>: "   + localehelper.timeFormat; height: 25  
                }

                Text { 
                    text: "<b>installedLocales</b>:"; height: 25  
                }
                Column {
                    Repeater {
                        model: localeModel
                        delegate:  Text {
                            text: "   - " + displayLocale + " [" + locale + "]"
                            width: paintedWidth + 10
                            height: 25
                        }
                    }
                }
            }
            Column {
                id: c1
                Text { text: "<b>Date Format</b>" ; font.pixelSize: 18; height: 25 }

                DropDown {
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
                Text { text: "<b>Time Format</b>" ; font.pixelSize: 18; height: 25 }

                DropDown {
                    height: 40

                    title: "Time Format"
                    showTitleInMenu: true
                    model:  [ "12 Hour",
                              "24 Hour"]
                    payload: [ Labs.LocaleHelper.TimeFormat12,
                               Labs.LocaleHelper.TimeFormat24 ]
                    selectedIndex: localehelper.timeFormat
                    onTriggered:  {
                        localehelper.timeFormat = payload[index]
                    }
                }

                LocaleHelperTime { label: "TimeFull";   format: Labs.LocaleHelper.TimeFull }
                LocaleHelperTime { label: "TimeFull12"; format: Labs.LocaleHelper.TimeFull12 }
                LocaleHelperTime { label: "TimeFull24"; format: Labs.LocaleHelper.TimeFull24 }

                Text { text: "<b>Signals</b>" ; font.pixelSize: 18; height: 25 }
                Rectangle {
                    width: 300
                    height: 100
                    color: "lightgray"
                    Flickable {
                        id: localeSignalsFlick
                        anchors.fill: parent

                        contentHeight: localeSignals.height
                        clip: true
                        focus: true
    
                        Text {
                            id: localeSignals
                            height: paintedHeight
                            width: paintedWidth
    
                            onTextChanged: {
                                localeSignalsFlick.contentY = Math.max(0, height - localeSignalsFlick.height)
                                
                            }
                        }
                    }
                }

            }
        }
    }
}
