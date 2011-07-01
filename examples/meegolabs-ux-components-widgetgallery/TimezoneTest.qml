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

    pageTitle: qsTr("Time Zone Test")

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    Labs.TimezoneListModel {
        id: tzModel
    }

    Flickable {
        id: flow
        anchors.fill: parent
        contentHeight: c0.height

        Column {
            id: c0
            Repeater {
                model: tzModel
                delegate:  Text {
                    text: "\"" + locationname + "\" [GMT offset: " + gmtoffset + "]"
                    height: 25
                }
            }
        }
    }
}
