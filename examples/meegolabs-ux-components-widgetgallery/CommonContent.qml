/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

/* This file is meant to show different kind of buttons.
   On clicked most buttons open another page where a detailed view is given. */

import Qt 4.7
import MeeGo.Components 0.1

Item {
    id: buttonContent

    anchors.fill: parent

    Rectangle {
        id: outerRect

        anchors.margins: parent.width * 0.01
        anchors.top:  parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color:  "darkgrey"
        radius:  5

        Flickable {
            id: flickContainer
            property int itemHeight: 80
            property int textMargin: 20
            property int textSize: 16
            property real leftFactor: 0.3
            property real rightFactor: 0.4
            property string backColor: "lightgrey"

            contentHeight: column.height
            anchors.fill: parent

            interactive: height < contentHeight
            onHeightChanged: { contentY = 0 }

            clip: true

            Column {
                id: column

                width: parent.width * 0.75
                anchors.horizontalCenter: parent.horizontalCenter

                spacing: 2

                Rectangle {
                    id: contactsPickerItem

                    width: parent.width
                    height:  flickContainer.itemHeight

                    anchors.left: parent.left

                    anchors.right: parent.right

                    border.width: 1
                    border.color: "grey"

                    color: flickContainer.backColor
                    clip:  true

                    MouseArea {
                        anchors.fill: contactsPickerItem
                        z: 1

                        onClicked: addPage( contactsPickerComponent )
                    }

                    Item {
                        x: parent.width / 2 - width - ( parent.width / 2 - width ) / 2
                        width:  parent.width  * flickContainer.leftFactor
                        anchors.verticalCenter: parent.verticalCenter
                        Button {
                            text:  qsTr("Contacts")

                            width: parent.width
                            anchors.centerIn: parent
                        }
                    }

                    Text {
                        x: parent.width / 2
                        width: parent.width * flickContainer.rightFactor
                        height: parent.height

                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                        font.pixelSize: flickContainer.textSize
                        text: qsTr("ContactsPicker, to choose from a list of contacts.")
                    }

                    Component { id: contactsPickerComponent; WidgetPageContactsPicker{} }
                }

                Rectangle {
                    id: timezoneListViewItem

                    width: parent.width
                    height:  flickContainer.itemHeight

                    anchors.left: parent.left

                    anchors.right: parent.right

                    border.width: 1
                    border.color: "grey"

                    color: flickContainer.backColor
                    clip:  true

                    MouseArea {
                        anchors.fill: timezoneListViewItem
                        z: 1

                        onClicked: addPage( timezoneListViewComponent )
                    }

                    Item {
                        x: parent.width / 2 - width - ( parent.width / 2 - width ) / 2
                        width:  parent.width  * flickContainer.leftFactor
                        anchors.verticalCenter: parent.verticalCenter
                        Button {
                            text:  qsTr("Time Zones")

                            width: parent.width
                            anchors.centerIn: parent
                        }
                    }

                    Text {
                        x: parent.width / 2
                        width: parent.width * flickContainer.rightFactor
                        height: parent.height

                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                        font.pixelSize: flickContainer.textSize
                        text: qsTr("TimezoneListView, to choose from a list of timezones.")
                    }

                    Component { id: timezoneListViewComponent; WidgetPageTimezoneListView{} }
                }
            }
        }
    } //end outerRect
}
