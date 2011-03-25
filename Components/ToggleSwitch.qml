/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Rectangle {
    id: toggleSwitch

    radius: 5
    color: "#505050"

    Text {
        id: amTextBg
        anchors.left: parent.left
        anchors.leftMargin: elementLabel.width
        anchors.verticalCenter: parent.verticalCenter
        text: onValue
        color: "darkgrey"
        font.pointSize: elementLabel.font.pointSize
    }
    Text {
        id: pmTextBg
        anchors.right: parent.right
        anchors.rightMargin: elementLabel.width
        anchors.verticalCenter: parent.verticalCenter
        text: offValue
        color: "darkgrey"
        font.pointSize: elementLabel.font.pointSize
    }

    signal toggled()

    property bool on: false
    property string onValue: qsTr("ON")
    property string offValue: qsTr("OFF")
    property string value: on ? onValue : offValue

    function toggle(ison)
    {
        on = ison
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            toggleSwitch.on = !toggleSwitch.on
            toggled()
        }
    }

    Image {
        id: toggleElement
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width/2
        source: "image/pnl_switch_blue_up.png"

        signal clicked()

        property int tempx: 0
        property bool pressed: false

        Text {
            id: elementLabel
            anchors.centerIn: parent
            text: toggleSwitch.on ? onValue : offValue
            color: "white"
            font.pixelSize: theme_fontPixelSizeMedium
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                toggleSwitch.on = !toggleSwitch.on
                toggled()
            }
            onPressed: {
                toggleElement.source = "image/pnl_switch_blue_dn.png"
                toggleElement.pressed = true

                toggleElement.tempx = mouseX
            }

            onReleased: {
                toggleElement.source = "image/pnl_switch_blue_up.png"
                toggleElement.pressed = false
            }

            onMousePositionChanged: {
                if(toggleElement.pressed) {
                    toggleElement.x -= toggleElement.tempx - mouseX;
                }
            }
        }
    }

    states: [
        State {
            name: onValue
            PropertyChanges { target: toggleElement; x: 0 }
            when: toggleSwitch.on == true
        },
        State {
            name: offValue
            PropertyChanges { target: toggleElement; x: parent.width - width }
            when: toggleSwitch.on == false
        },

        State {
            name: "draggedoff"
            PropertyChanges { target: toggleSwitch; on: false }
            when: !toggleElement.pressed && !(!toggleSwitch.on && (toggleElement.x + toggleElement.width / 2) < toggleSwitch.width / 2)
        },
        State {
            name: "draggedon"
            PropertyChanges { target: toggleSwitch; on: true }
            when: !toggleElement.pressed && (!toggleSwitch.on && (toggleElement.x + toggleElement.width / 2) < toggleSwitch.width / 2)
        }

    ]

    transitions: [
        Transition {
            NumberAnimation {
                properties: "x"
                duration: 200
                easing.type: Easing.InCubic
            }

        }
    ]
}





