/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

/* CheckBoxEntry
 * This file combines a Text item and a CheckBox item meant as
 * a template to add a control element to the WidgetPages which
 * offers input for a boolean value.
 */

import Qt 4.7
import MeeGo.Components 0.1

Item {
    id: item

    property alias label: label.text
    property alias isChecked: checkBox.isChecked

    signal checkedChanged( bool checked )

    anchors { left: parent.left; right: parent.right }
    height:  50

    Text {
        id: label

        anchors { left: parent.left; right: parent.horizontalCenter; top: parent.top; bottom: parent.bottom; margins: 5 }
        verticalAlignment: "AlignVCenter"
        horizontalAlignment: "AlignLeft"
        font.pixelSize: parent.height * 0.5
        elide: Text.ElideRight
        text: qsTr("value: ")
    }

    CheckBox {
        id: checkBox

        anchors { left: parent.horizontalCenter; top: parent.top; bottom: parent.bottom; margins: 5 }
        width: height

        onIsCheckedChanged: {
            item.checkedChanged( isChecked )
        }
    }
}
