/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

/* StatusEntry
 * This file combines two Text items meant as a template to
 * show status output for a widget.
 */

import Qt 4.7
import MeeGo.Components 0.1

Item {
    id: outputItem

    property alias label: outputText1.text
    property alias value: outputText2.text

    width: outputText1.paintedWidth + outputText2.paintedWidth  + outputText2.anchors.leftMargin
    height: 50

    Text {
        id: outputText1

        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: "AlignVCenter"
        horizontalAlignment: "AlignLeft"
        font.pixelSize: parent.height * 0.5
    }

    Text {
        id: outputText2

        anchors { left: outputText1.right; leftMargin: 20; verticalCenter: outputText1.verticalCenter }
        verticalAlignment: "AlignVCenter"
        horizontalAlignment: "AlignLeft"
        font.pixelSize: parent.height * 0.5
    }
}
