/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Item {
    id: container
    width: 110
    height: 130

    property double margin : 15

    signal clicked
    signal pressAndHold(variant mouseX, variant mouseY)

    Item {
        width: parent.width
        height: parent.height - 20
        Image {
            anchors.centerIn: parent
            source: icon
        }
    }

    Text {
        id: iconTextBox
        width: parent.width + 20
        height: 25
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: title 
        font.bold: true
        font.pixelSize: theme_iconFontPixelSize
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: theme_iconFontColor
        elide: Text.ElideRight
    }
    MouseArea {
        anchors.fill: parent
        onClicked: container.clicked()
        onPressAndHold: container.pressAndHold(mouseX, mouseY)
    }
}
