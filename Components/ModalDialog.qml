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

    property alias contentLoader: contentLoader
    property alias leftButtonText: button1.title
    property alias middleButtonText: button3.title
    property alias rightButtonText: button2.title
    property alias bgSourceUpLeft: button1.bgSourceUp
    property alias bgSourceDnLeft: button1.bgSourceDn
    property alias bgSourceUpRight: button2.bgSourceUp
    property alias bgSourceDnRight: button2.bgSourceDn
    property alias bgSourceUpMiddle: button3.bgSourceUp
    property alias bgSourceDnMiddle: button3.bgSourceDn
    property alias dialogTitle: title.text
    property alias dialogHeight: contents.height
    property alias dialogWidth: contents.width

    anchors.fill: parent

    signal dialogClicked (int button)

    Rectangle {
        id: fog

        anchors.fill: parent
        color: theme_dialogFogColor
        opacity: theme_dialogFogOpacity
        Behavior on opacity {
            PropertyAnimation { duration: theme_dialogAnimationDuration }
        }
    }

    /* This mousearea is to prevent clicks from passing through the fog */
    MouseArea {
        anchors.fill: parent
    }

    BorderImage {
        id: dialog

        border.top: 14
        border.left: 20
        border.right: 20
        border.bottom: 20

        source: "image://theme/notificationBox_bg"

        anchors.centerIn: parent

        width: contents.width + 40 //478
        height: contents.height + 40 //318

        Item {
            id: contents

            anchors.centerIn: parent

            width: 438
            height: 220

            Column {
                id: contentColumn
                width: {
                    if (childrenRect.width > contents.width)
                        contents.width = childrenRect.width;

                    contents.width
                }

                Text {
                    anchors.left: parent.left
                    id: title
                    text: ""
                    font.weight: Font.Bold
                    font.pixelSize: theme_dialogTitleFontPixelSize
                    color: theme_dialogTitleFontColor
                }

                Loader {
                    id: contentLoader
                    width: parent.width
                    height: contents.height - (buttonBar.height + title.height)
                }

                Row {
                    id: buttonBar
                    height: childrenRect.height
                    spacing: 18
                    anchors.horizontalCenter: parent.horizontalCenter

                    Button {
                        id: button1
                        width: 210
                        height: 60
                        visible: button1.title.length > 0
                        onClicked: {
                            container.dialogClicked (1);
                        }
                    }

                    Button {
                        id: button3
                        width: button1.width
                        height: button1.height
                        visible: button3.title.length > 0
                        onClicked: {
                            container.dialogClicked (3);
                        }
                    }
                    Button {
                        id: button2
                        width: button1.width
                        height: button1.height
                        visible: button2.title.length > 0
                        onClicked: {
                            container.dialogClicked (2);
                        }
                    }
                }
            }
        }
    }
}
