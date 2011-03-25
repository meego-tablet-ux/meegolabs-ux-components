/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Fog {

    id: pickerDialog

    // API
    property alias title: titleText.text

    property alias contentWidth: dialogArea.width
    property alias contentHeight: dialogArea.height
    property alias content: contentField.children
    property alias buttons: buttonField.children

    // Private
    modalSurface: Rectangle {

        id: dialogArea

        anchors.centerIn: parent
        width: parent.width * 0.90
        height: parent.height * 0.90

        opacity: pickerDialog.show ? 1 : 0
        visible: opacity > 0

        color: theme_dialogBackgroundColor

        MouseArea {
            anchors.fill: parent
            onClicked: {
                // ck todo no button: close
            }
        }

        BorderImage {

            id: titleBar

            anchors { left: parent.left;
                      top: parent.top;
                      right: parent.right
            }
            anchors.horizontalCenter: parent.horizontalCenter
            height: 35

            source: "image://theme/titlebar_l"

            Text {

                id: titleText

                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                width: parent.width
                horizontalAlignment: Text.AlignHCenter

                color: theme_dialogTitleFontColor
                elide: Text.ElideRight
                font.pixelSize: theme_dialogTitleFontPixelSize // title site

            }
        }

        BorderImage {

            id: contentArea

            clip:  true
            anchors {   left: parent.left;
                        right: parent.right;
                        top: titleBar.bottom;
                        bottom: parent.bottom;
                    }

            source: "image://theme/titlebar_l" //ck

            Item { // placeholder for content

                id: contentField

                anchors {   top: parent.top;
                            bottom: buttonField.top;
                            left: parent.left;
                            right: parent.right;
                            leftMargin: 5
                            rightMargin: 5
                }

            } // contentField

            Item { // placeholder for buttons

                id: buttonField

                anchors { left: parent.left;
                          right: parent.right;
                          bottom: parent.bottom;
                }
                height: 60
                width: parent.width
            } // buttonField
        }

        Behavior on opacity { NumberAnimation { duration: theme_dialogAnimationDuration } }
    }
}
