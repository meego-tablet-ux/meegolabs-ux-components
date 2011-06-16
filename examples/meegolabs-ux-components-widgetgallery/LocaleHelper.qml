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
                Text { id: c1;  text: "Date" ; width: 500; height: 25 }
                Text { id: l1;  text: "FullLong: "   + localehelper.currentDate( Labs.LocaleHelper.DateFullLong ); width: 500; height: 25  }
                Text { id: l2;  text: "Full: "       + localehelper.currentDate( Labs.LocaleHelper.DateFull); width: 500; height: 25  }
                Text { id: l3;  text: "FullShort: "      + localehelper.currentDate( Labs.LocaleHelper.DateFullShort); width: 500; height: 25  }
                Text { id: l4;  text: "FullNum: "    + localehelper.currentDate( Labs.LocaleHelper.DateFullNum ); width: 500; height: 25  }
                Text { id: l5;  text: "FullNumShort: " + localehelper.currentDate( Labs.LocaleHelper.DateNumShort); width: 500; height: 25  }
                Text { id: l6;  text: "DateWeekdayMonthDay: " + localehelper.currentDate( Labs.LocaleHelper.DateWeekdayMonthDay ); width: 500; height: 25  }
                Text { id: l7;  text: "DateWeekdayDayShort: " + localehelper.currentDate( Labs.LocaleHelper.DateWeekdayDayShort ); width: 500; height: 25  }
                Text { id: l8;  text: "DateMonthDay: " + localehelper.currentDate( Labs.LocaleHelper.DateMonthDay ); width: 500; height: 25  }
                Text { id: l9;  text: "DateMonthYear: " + localehelper.currentDate( Labs.LocaleHelper.DateMonthYear ); width: 500; height: 25  }
                Text { id: l10; text: "DateMonthYearShort: " + localehelper.currentDate( Labs.LocaleHelper.DateMonthYearShort ); width: 500; height: 25  }
                Text { id: l11; text: "DateDay: " + localehelper.currentDate( Labs.LocaleHelper.DateDay ); width: 500; height: 25  }
                Text { id: l12; text: "DateWeekday: " + localehelper.currentDate( Labs.LocaleHelper.DateWeekday ); width: 500; height: 25  }
                Text { id: l13; text: "DateWeekdayShort: " + localehelper.currentDate( Labs.LocaleHelper.DateWeekdayShort ); width: 500; height: 25  }
                Text { id: l14; text: "DateMonth: " + localehelper.currentDate( Labs.LocaleHelper.DateMonth ); width: 500; height: 25  }
                Text { id: l15; text: "DateMonthShort: " + localehelper.currentDate( Labs.LocaleHelper.DateMonthShort ); width: 500; height: 25  }
                Text { id: l16; text: "DateYear: " + localehelper.currentDate( Labs.LocaleHelper.DateYear ); width: 500; height: 25  }
            }
            Column {
                Text { id: c2;  text: "Time" ; width: 500; height: 25 }
                Text { id: l21;  text: "TimeFull: "   + localehelper.currentTime( Labs.LocaleHelper.TimeFull ); width: 500; height: 25  }
                Text { id: l22;  text: "TimeFull12: "       + localehelper.currentTime( Labs.LocaleHelper.TimeFull12); width: 500; height: 25  }
                Text { id: l23;  text: "TimeFull24: "      + localehelper.currentTime( Labs.LocaleHelper.TimeFull24); width: 500; height: 25  }
                Text { id: l24;  text: "TimeQtLong: "    + localehelper.currentTime( Labs.LocaleHelper.TimeQtLong ); width: 500; height: 25  }
                Text { id: l25;  text: "TimeQtShort: " + localehelper.currentTime( Labs.LocaleHelper.TimeQtShort); width: 500; height: 25  }
                Text { id: l26;  text: "DateTime" ; width: 500; height: 25  }
                Text { id: l27;  text: "DateTimeQtLong: " + localehelper.currentDateTime( Labs.LocaleHelper.DateTimeQtLong ); width: 500; height: 25  }
                Text { id: l28;  text: "DateTimeQtShort: " + localehelper.currentDateTime( Labs.LocaleHelper.DateTimeQtShort ); width: 500; height: 25  }
            }
        }
    }


    Labs.LocaleHelper {
        id: localehelper
    }

}
