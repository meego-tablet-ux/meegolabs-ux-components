/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Image {
    id: timeToggleSwitch

    source: "image://theme/pickers/pnl_switch_bg"

    anchors.fill: parent

    Text {
        id: amTextBg
        anchors.left: parent.left
        anchors.leftMargin: elementLabel.width
        anchors.verticalCenter: parent.verticalCenter
        text: onValue
        color: theme_fontColorNormal
        font.pointSize: elementLabel.font.pointSize
    }
    Text {
        id: pmTextBg
        anchors.right: parent.right
        anchors.rightMargin: elementLabel.width
        anchors.verticalCenter: parent.verticalCenter
        text: offValue
        color: theme_fontColorNormal
        font.pointSize: elementLabel.font.pointSize
    }

    signal toggled()

    property bool on: false
    property string onValue: qsTr("ON")
    property string offValue: qsTr("OFF")

    function toggle(ison)
    {
        on = ison
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            timeToggleSwitch.on = !timeToggleSwitch.on
            toggled()
        }
    }

    Image {
        id: toggleElement
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width/2
        source: "image://theme/pickers/pnl_switch_blue_up"

        signal clicked()

        property int tempx: 0
        property bool pressed: false

        Text {
            id: elementLabel
            anchors.centerIn: parent
            text: timeToggleSwitch.on ? onValue : offValue
            color: theme_fontColorHighlight
            font.pixelSize: theme_fontPixelSizeMedium
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                timeToggleSwitch.on = !timeToggleSwitch.on
                toggled()
            }
            onPressed: {
                toggleElement.source = "image://theme/pickers/pnl_switch_blue_dn"
                toggleElement.pressed = true

                toggleElement.tempx = mouseX
            }

            onReleased: {
                toggleElement.source = "image://theme/pickers/pnl_switch_blue_up"
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
            PropertyChanges {
                target: toggleElement
                x: 0
            }

            when: timeToggleSwitch.on == true
        },
        State {
            name: offValue
            PropertyChanges {
                target: toggleElement
                x: parent.width - width
            }

            when: timeToggleSwitch.on == false
        },

        State {
            name: "draggedoff"
            PropertyChanges {
                target: timeToggleSwitch
                on: false
            }
            when: !toggleElement.pressed && !(!timeToggleSwitch.on && (toggleElement.x + toggleElement.width / 2) < timeToggleSwitch.width / 2)
        },

        State {
            name: "draggedon"
            PropertyChanges {
                target: timeToggleSwitch
                on: true
            }

            when: !toggleElement.pressed && (!timeToggleSwitch.on && (toggleElement.x + toggleElement.width / 2) < timeToggleSwitch.width / 2)
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





