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

    property alias title: label.text
    property alias font: label.font
    property alias color: label.color
    property string bgSourceUp: "image://theme/btn_grey_up"
    property string bgSourceDn: "image://theme/btn_grey_dn"
    property bool pressed: false
    property bool active: true
    opacity: active ? 1.0 : 0.5

    width: 0
    height: 0

    signal clicked(variant mouse)

    BorderImage {
        id: icon
        border.bottom: 10
        border.top: 10
        border.left: 10
        border.right: 10
        source: bgSourceUp

        anchors.fill: parent

        states: [
            State {
                name: "pressed"
                when: container.pressed
                PropertyChanges {
                    target: icon
                    source: bgSourceDn
                }
            }
        ]

    }

    Text {
        id: label
        anchors.centerIn: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideRight
        font.pixelSize: theme_fontPixelSizeLargest
        color: theme_buttonFontColor

        Component.onCompleted: {
            if (container.width == 0)
                container.width = label.paintedWidth + 20;

            if (container.height == 0)
                container.height = label.paintedHeight + 20;
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (container.active)
            {
                container.clicked(mouse)
            }
        }
        onPressed: if (container.active) parent.pressed = true
        onReleased: if (container.active) parent.pressed = false
    }
}
