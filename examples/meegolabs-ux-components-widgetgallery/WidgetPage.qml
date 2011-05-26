/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

/* This page is meant to show one widget and offer controls to manipulate its size, color
 * and whatever else makes sense. Additionally a description should be written for the widget.
 *
 * Properties:
 *      controlContent: expects a list of control elements to set them as children for a column.
 *      statusContent: expects a list of elements which display status output of the widget
 *      description: expects a string which contains the description of the widget
 *      widget: expects an object of the widget to be shown on this page
 */

import Qt 4.7
import MeeGo.Components 0.1

AppPage {
    id: mainPage

    property alias controlContent: controlColumn.children //put control elements here
    property alias statusContent: statusColumn.children //put control elements here
    property alias description: descriptionText.text  //put text here which desribes the shown widget
    property alias widget: widgetContainer.children //put widget here that shall be shown on this page

    //upper screen area, contains control elements and description
    Item {
        id: controlItem

        anchors { top: parent.top; bottom: parent.verticalCenter; left: parent.left; right: parent.right; margins: 1 }

        //contains control elements to manipulate the widget
        Rectangle {
            id: controlRect

            anchors { top: parent.top; left: parent.left; bottom: parent.bottom; right: parent.horizontalCenter; margins: 1 }
            color:  "lightgrey"

            Rectangle {
                id: header
                anchors { top: parent.top; left: parent.left; right: parent.right; margins: 2 }
                height: childrenRect.height
                color: "silver"

                Text {
                    text: qsTr("API Properties")
                    font.pixelSize: 26
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: 4
                }
            }

            Flickable {
                id: controlElements

                anchors { top: header.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }
                anchors.bottomMargin: 6
                flickableDirection: Flickable.VerticalFlick
                clip: true
                contentHeight: controlColumn.childrenRect.height + 20
                interactive: contentHeight > height

                Column {
                    id: controlColumn

                    anchors { fill: parent; margins: 10 }
                }
            }
        }

        //contains a description for the widget
        Rectangle {
            id: descriptionRect

            anchors { top: parent.top; right: parent.right; bottom: parent.bottom; left: parent.horizontalCenter; margins: 1 }
            color:  "lightgrey"

            Rectangle {
                id: descriptionBackground

                anchors.top: parent.top
                anchors.right: parent.right
                anchors.bottom: statusBackground.top
                anchors.left: parent.left
                anchors.margins: 2
                color: "darkgray"

                Flickable {
                    id: descriptionFlickable

                    anchors.fill: parent
                    anchors.margins: 4
                    flickableDirection: Flickable.VerticalFlick
                    clip: true
                    contentHeight: descriptionText.height + 6
                    interactive: contentHeight > height

                    Text {
                        id: descriptionText

                        text: qsTr("Put description here...")
                        font.pixelSize: 20
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: 4
                        wrapMode: Text.WordWrap
                    }
                }
            }

            Rectangle {
                id: statusBackground

                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: 2
                anchors.rightMargin: 2
                anchors.bottomMargin: 2
                color: "darkgray"
                height: parent.height / 2

                Flickable {
                    id: statusFlickable

                    property bool hFlick: contentWidth > width
                    property bool vFlick: contentHeight > height

                    anchors.fill: parent
                    anchors.margins: 4
                    flickableDirection: if( hFlick && vFlick ) {
                                            return Flickable.HorizontalAndVerticalFlick
                                        }else if( hFlick ) {
                                            return Flickable.HorizontalFlick
                                        }else{
                                            return Flickable.VerticalFlick
                                        }
                    clip: true
                    contentHeight: statusColumn.height + 0
                    contentWidth: statusColumn.width + 0
                    interactive: hFlick || vFlick

                    Column {
                        id: statusColumn

                        width:  childrenRect.width
                        anchors.top: parent.top
                        anchors.left: parent.left
                    }
                }
            }
        }
    }

    //lower screen area, shows a widget and should contain enough space to play around with its size
    Rectangle {
        id: widgetRect

        anchors.top: parent.verticalCenter
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 2
        anchors.rightMargin: 2
        anchors.bottomMargin: 2

        color:  "lightgrey"

        Item {
            id:  widgetContainer
            anchors { fill: parent; margins: 10 }
        }
    }

}
