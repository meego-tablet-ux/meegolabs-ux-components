/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Ux.Components.Common 0.1
import MeeGo.Labs.Components 0.1 as Labs

Text {
    id: date
    property string label: ""
    property int format: 0

    width: 300; height: 25
    text: displayText()

    function displayText() {
        return format ? ("<b>" + label + "</b>: " + localehelper.currentTime(format)) : ""
    }

    Connections {
        target: localehelper
        onTimeFormatChanged: {
            text = displayText()
        }
    }
}
