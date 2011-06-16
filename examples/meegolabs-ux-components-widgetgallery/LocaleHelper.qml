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

        Row {
            Column {
                Text { id: c0;  text: "<b>General</b>" ; font.pixelSize: 18; width: 300; height: 40 }
                Text { text: "name: "   + localehelper.name; width: 300; height: 25  }
                Text { text: "country: "   + localehelper.country; width: 300; height: 25  }
                Text { text: "decimalPoint: "   + localehelper.decimalPoint(); width: 300; height: 25  }
                Text { text: "firstDayOfWeek: "   + localehelper.firstDayOfWeek(); width: 300; height: 25  }
                Text { text: "numericDateOrder: "   + localehelper.numericDateOrder(); width: 300; height: 25  }
            }
            Column {
                Text { id: c1;  text: "<b>Date</b>" ; font.pixelSize: 18; width: 300; height: 40 }
                Text { id: l1;  text: "FullLong: "   + localehelper.currentDate( Labs.LocaleHelper.DateFullLong ); width: 300; height: 25  }
                Text { id: l2;  text: "Full: "       + localehelper.currentDate( Labs.LocaleHelper.DateFull); width: 300; height: 25  }
                Text { id: l3;  text: "FullShort: "      + localehelper.currentDate( Labs.LocaleHelper.DateFullShort); width: 300; height: 25  }
                Text { id: l4;  text: "FullNum: "    + localehelper.currentDate( Labs.LocaleHelper.DateFullNum ); width: 300; height: 25  }
                Text { id: l5;  text: "FullNumShort: " + localehelper.currentDate( Labs.LocaleHelper.DateNumShort); width: 300; height: 25  }
                Text { id: l6;  text: "DateWeekdayMonthDay: " + localehelper.currentDate( Labs.LocaleHelper.DateWeekdayMonthDay ); width: 300; height: 25  }
                Text { id: l7;  text: "DateWeekdayDayShort: " + localehelper.currentDate( Labs.LocaleHelper.DateWeekdayDayShort ); width: 300; height: 25  }
                Text { id: l8;  text: "DateMonthDay: " + localehelper.currentDate( Labs.LocaleHelper.DateMonthDay ); width: 300; height: 25  }
                Text { id: l9;  text: "DateMonthYear: " + localehelper.currentDate( Labs.LocaleHelper.DateMonthYear ); width: 300; height: 25  }
                Text { id: l10; text: "DateMonthYearShort: " + localehelper.currentDate( Labs.LocaleHelper.DateMonthYearShort ); width: 300; height: 25  }
                Text { id: l11; text: "DateDay: " + localehelper.currentDate( Labs.LocaleHelper.DateDay ); width: 300; height: 25  }
                Text { id: l12; text: "DateWeekday: " + localehelper.currentDate( Labs.LocaleHelper.DateWeekday ); width: 300; height: 25  }
                Text { id: l13; text: "DateWeekdayShort: " + localehelper.currentDate( Labs.LocaleHelper.DateWeekdayShort ); width: 300; height: 25  }
                Text { id: l14; text: "DateMonth: " + localehelper.currentDate( Labs.LocaleHelper.DateMonth ); width: 300; height: 25  }
                Text { id: l15; text: "DateMonthShort: " + localehelper.currentDate( Labs.LocaleHelper.DateMonthShort ); width: 300; height: 25  }
                Text { id: l16; text: "DateYear: " + localehelper.currentDate( Labs.LocaleHelper.DateYear ); width: 300; height: 25  }
            }
            Column {
                Text { id: c2;  text: "<b>Time</b>" ; font.pixelSize: 18; width: 300; height: 40 }
                Text { id: l21;  text: "TimeFull: "   + localehelper.currentTime( Labs.LocaleHelper.TimeFull ); width: 300; height: 25  }
                Text { id: l22;  text: "TimeFull12: "       + localehelper.currentTime( Labs.LocaleHelper.TimeFull12); width: 300; height: 25  }
                Text { id: l23;  text: "TimeFull24: "      + localehelper.currentTime( Labs.LocaleHelper.TimeFull24); width: 300; height: 25  }
                Text { id: l24;  text: "TimeQtLong: "    + localehelper.currentTime( Labs.LocaleHelper.TimeQtLong ); width: 300; height: 25  }
                Text { id: l25;  text: "TimeQtShort: " + localehelper.currentTime( Labs.LocaleHelper.TimeQtShort); width: 300; height: 25  }
                Text { id: l26;  text: "DateTime" ; width: 300; height: 25  }
                Text { id: l27;  text: "DateTimeQtLong: " + localehelper.currentDateTime( Labs.LocaleHelper.DateTimeQtLong ); width: 300; height: 25  }
                Text { id: l28;  text: "DateTimeQtShort: " + localehelper.currentDateTime( Labs.LocaleHelper.DateTimeQtShort ); width: 300; height: 25  }
            }
        }
    }


    Labs.LocaleHelper {
        id: localehelper
    }

}
