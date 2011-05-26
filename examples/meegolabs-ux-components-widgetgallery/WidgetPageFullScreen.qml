/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

/* This pages can call up the ExpandingBox.qml and offers controls to manipulate it.
 */

import Qt 4.7
import MeeGo.Components 0.1

WidgetPage {
    id: widgetPage

    pageTitle: "Fullscreen Testing"

    controlContent: [
        Item {
            id: fullScreenBox

            anchors { left: parent.left; right: parent.right }
            height:  50

            Text {
                id: label

                anchors { left: parent.left; right: parent.horizontalCenter; top: parent.top; bottom: parent.bottom; margins: 5 }
                verticalAlignment: "AlignVCenter"
                horizontalAlignment: "AlignLeft"
                font.pixelSize: parent.height * 0.5
                elide: Text.ElideRight
                text: qsTr("fullScreen: " )
            }

            Text {
                id: textEntry

                anchors { left: parent.horizontalCenter; right: parent.right; top: parent.top; bottom: parent.bottom; margins: 5 }
                verticalAlignment: "AlignVCenter"
                font.pixelSize: parent.height * 0.5
                elide: Text.ElideRight
                text: widgetPage.fullScreen
            }
        },

        Item {
            id: fullContentBox

            anchors { left: parent.left; right: parent.right }
            height:  50

            Text {
                id: label2

                anchors { left: parent.left; right: parent.horizontalCenter; top: parent.top; bottom: parent.bottom; margins: 5 }
                verticalAlignment: "AlignVCenter"
                horizontalAlignment: "AlignLeft"
                font.pixelSize: parent.height * 0.5
                elide: Text.ElideRight
                text: qsTr("fullContent: " )
            }

            Text {
                id: textEntry2

                anchors { left: parent.horizontalCenter; right: parent.right; top: parent.top; bottom: parent.bottom; margins: 5 }
                verticalAlignment: "AlignVCenter"
                font.pixelSize: parent.height * 0.5
                elide: Text.ElideRight
                text: widgetPage.fullContent
            }
        },

        Item {
            id: fullScreenBox2

            anchors { left: parent.left; right: parent.right }
            height:  50

            Text {
                id: label3

                anchors { left: parent.left; right: parent.horizontalCenter; top: parent.top; bottom: parent.bottom; margins: 5 }
                verticalAlignment: "AlignVCenter"
                horizontalAlignment: "AlignLeft"
                font.pixelSize: parent.height * 0.5
                elide: Text.ElideRight
                text: qsTr("pageUsingFullScreen: " )
            }

            Text {
                id: pageFullscreen

                anchors { left: parent.horizontalCenter; right: parent.right; top: parent.top; bottom: parent.bottom; margins: 5 }
                verticalAlignment: "AlignVCenter"
                font.pixelSize: parent.height * 0.5
                elide: Text.ElideRight
                text: widgetPage.pageUsingFullScreen
            }
        }
    ]

    description: qsTr("Here you can test the different fullscreen modes.")

    widget: Flickable {

        anchors.fill: parent
        contentHeight: height > buttonBox.height ? height : buttonBox.height
        contentWidth: width > buttonBox.width ? width : buttonBox.width
        clip: true

        Column{
            id: buttonBox

            anchors.centerIn: parent
            spacing: 10
            Button {
                id: ebox1


                text: widgetPage.fullScreen ? qsTr("End Fullscreen") : qsTr("Fullscreen")

                onClicked: {
                    widgetPage.fullScreen = widgetPage.fullScreen ? false : true
                }
            }

            Button {
                id: ebox2


                text: widgetPage.fullContent ? qsTr("End FullContent") : qsTr("FullContent")

                onClicked: {
                    widgetPage.fullContent = widgetPage.fullContent ? false : true
                }
            }

            Button {
                id: ebox3


                text: widgetPage.pageUsingFullScreen ? qsTr("End Page using Fullscreen") : qsTr("Page using Fullscreen")

                onClicked: {
                    widgetPage.pageUsingFullScreen = widgetPage.pageUsingFullScreen ? false : true
                }
            }
        }
    }

    TopItem {
        id: topItem
    }
}


