/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

/* LabelEntry
 * This file combines a Text item and a TextEntry item meant as
 * a template to add control element to the WidgetPages which
 * react to text input.
 */

import Qt 4.7
import MeeGo.Components 0.1

Item {
    id: item

    property alias label: label.text
    property alias defaultValue: textEntry.defaultText
    property alias text: textEntry.text
    property alias labelColor: label.color

    signal textUpdated( variant value )

    anchors { left: parent.left; right: parent.right }
    height:  50

    Text {
        id: label

        anchors { left: parent.left; right: parent.horizontalCenter; top: parent.top; bottom: parent.bottom; margins: 5 }
        verticalAlignment: "AlignVCenter"
        horizontalAlignment: "AlignLeft"
        font.pixelSize: parent.height * 0.5
        elide: Text.ElideRight
        text: "value: "
    }

    TextEntry {
        id: textEntry

        clip: true
        anchors { left: parent.horizontalCenter; right: parent.right; top: parent.top; bottom: parent.bottom; margins: 5 }
        defaultText: "0"

        onTextChanged: { item.textUpdated( text ) }
    }
}
