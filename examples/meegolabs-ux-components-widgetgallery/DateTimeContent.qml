/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

/* This page shows the types of modal dialogs available. On clicked
   the icons open further pages with detailed information. */

import Qt 4.7
import MeeGo.Components 0.1

Item {
    id: pickerContent

    anchors.fill: parent

    Rectangle {
        id: outerRect

        anchors.margins: parent.width * 0.01
        anchors.top:  parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width:  parent.width * 0.75
        color:  "darkgrey"
        radius:  5

        function showPage ( type ){
            if( type == "photo" ){
                addPage( photoPickerPage )
            }
            else if( type == "video" ){
                addPage( videoPickerPage )
            }
            else if( type == "music" ){
                addPage( musicPickerPage )
            }
            else if( type == "date" ){
                addPage( datePickerPage )
            }
            else if( type == "time" ){
                addPage( timePickerPage )
            }
            else if( type == "message" ){
                addPage( messageBoxPage )
            }
        }

        Flickable {
            id: flickContainer

            property int itemHeight: 80
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

//                Repeater {
//                    id: pickerRepeater

//                    width: parent.width
//                    model:  buttonModel

//                    Rectangle {
//                        id: buttonItem

//                        width: parent.width
//                        height:  flickContainer.itemHeight

//                        anchors.left: parent.left

//                        anchors.right: parent.right

//                        border.width: 1
//                        border.color: "grey"

//                        color: flickContainer.backColor
//                        clip:  true

//                        MouseArea {
//                            anchors.fill: buttonItem
//                            z: 1

//                            onClicked: outerRect.showPage( pickerType )
//                        }

//                        Item {
//                            id: button

//                            x: parent.width / 2 - width - ( parent.width / 2 - width ) / 2
//                            width:  parent.width  * flickContainer.leftFactor
//                            anchors.verticalCenter: parent.verticalCenter
//                            Button {
//                                text:  pickerType

//                                width: parent.width
//                                anchors.centerIn: parent
//                                elideText: true
//                            }
//                        }

//                        Text {
//                            x: parent.width / 2
//                            width: parent.width * flickContainer.rightFactor
//                            height: parent.height

//                            verticalAlignment: Text.AlignVCenter
//                            wrapMode: Text.WordWrap
//                            font.pixelSize: flickContainer.textSize
//                            text: legend
//                        }

//                        Component { id: buttonComponent; WidgetPageButton{} }
//                    }
//                }
            }//end leftColumn
        }

//        Component { id: messageBoxPage; WidgetPageMessageBox{} }
//        Component { id: photoPickerPage; WidgetPagePhotoPicker{} }
//        Component { id: musicPickerPage; WidgetPageMusicPicker{} }
//        Component { id: videoPickerPage; WidgetPageVideoPicker{} }
//        Component { id: datePickerPage; WidgetPageDatePicker {} }
//        Component { id: timePickerPage; WidgetPageTimePicker {} }

        //model for the left column
        ListModel {
            id: buttonModel

            ListElement {
                listIcon: "image://themedimage/widgets/common/camera/camera_lens_sm_up"
                listIconDown: "image://themedimage/widgets/common/camera/camera_lens_sm_dn"
                pickerType: "date"
                legend: "DatePicker, to choose or set a date."
            }
            ListElement {
                listIcon: "image://themedimage/widgets/common/camera/camera_lens_sm_up"
                listIconDown: "image://themedimage/widgets/common/camera/camera_lens_sm_dn"
                pickerType: "time"
                legend: "TimePicker, to choose or set a time."
            }
        }
    }
} //end outerRect
