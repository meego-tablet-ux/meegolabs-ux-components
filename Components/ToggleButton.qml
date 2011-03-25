/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Image {
    id: toggleSwitch

    source: "image://theme/panels/pnl_switch_bg"

    property bool on: false
    property alias color: toggleElement.color

    property bool suppress: true

    signal toggled(bool isOn);

    property alias onLabel: elementLabelOn.text
    property alias offLabel: elementLabelOff.text

    function toggle()
    {
        toggleSwitch.on = !toggleSwitch.on
        toggleSwitch.toggled(toggleSwitch.on);
    }

    MouseArea {
        anchors.fill: parent
        onClicked: toggleSwitch.toggle()
    }

	Text {
		id: elementLabelOn
		anchors.verticalCenter: parent.verticalCenter
		anchors.left: parent.left
		anchors.leftMargin: 15
		text: qsTr("On")
		visible: toggleSwitch.on
		color: "#2fa7d4"
		font.pointSize: toggleElement.height < toggleElement.width ? toggleElement.height/3 : toggleElement.width/4
	}

	Text {
		id: elementLabelOff
		anchors.verticalCenter: parent.verticalCenter
		anchors.right: parent.right
		anchors.rightMargin: 15
		text: qsTr("Off")
		visible: !toggleSwitch.on
		//color: theme_fontToggleSwitchColorOff
		color: "black"
		font.pointSize: toggleElement.height < toggleElement.width ? toggleElement.height/3 : toggleElement.width/4
	}

    Image {
        id: toggleElement

        anchors.top: parent.top

        source: imageup

        signal clicked()

        property int tempx: 0
        property bool pressed: false
        property string color: "blue"

        property string imageup: "image://theme/panels/pnl_switch_" + color + "_up"
        property string imagedn: "image://theme/panels/pnl_switch_" + color + "_dn"

        MouseArea {
            anchors.fill: parent
            onClicked: toggleSwitch.toggle() //toggleSwitch.on = !toggleSwitch.on
            onPressed: {
                toggleElement.source = toggleElement.imagedn
                toggleElement.pressed = true

                toggleElement.tempx = mouseX
            }

            onReleased: {
                toggleElement.source = toggleElement.imageup
                toggleElement.pressed = false
            }

            /*onMousePositionChanged: {
                if(toggleElement.pressed) {
                    toggleElement.x -= toggleElement.tempx - mouseX;
                    console.log((!toggleSwitch.on && (toggleElement.x + toggleElement.width / 2) < toggleSwitch.width / 2))
                }
            }*/
        }
    }


    states: [
        State {
            name: "on"
            PropertyChanges {
                target: toggleElement
				x: parent.width - width
            }
            when: toggleSwitch.on == true
        },
        State {
            name: "off"
            PropertyChanges {
                target: toggleElement
				x: 0
            }
            when: toggleSwitch.on == false
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





