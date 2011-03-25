/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Fog {

    id: messageBox

    // API
    property alias title: titleText.text
    property alias message: messageText.text
    property alias buttons : buttonField.children // usage overwrites the standard buttons!

    property alias contentWidth: dialogArea.width
    property alias contentHeight: dialogArea.height

    property alias showAcceptButton : buttonField.showAcceptButton
    property alias acceptButtonText : buttonField.acceptButtonText
    property alias acceptButtonActive : buttonField.acceptButtonActive

    property alias showCancelButton : buttonField.showCancelButton
    property alias cancelButtonText : buttonField.cancelButtonText
    property alias cancelButtonActive : buttonField.cancelButtonActive

    property alias showRejectButton : buttonField.showRejectButton
    property alias rejectButtonText : buttonField.rejectButtonText
    property alias rejectButtonActive : buttonField.rejectButtonActive

    signal accepted
    signal rejected
    signal canceled

    // Private
    modalSurface: Rectangle {

        id: dialogArea

        anchors.centerIn: parent

        opacity: fogShow ? 1 : 0
        visible: opacity > 0

        color: theme_dialogBackgroundColor

        BorderImage {

            id: titleBar

            anchors { left: parent.left; top: parent.top; right: parent.right }
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

        MouseArea {
            id: dialogMouseArea

            anchors.fill: parent
            onClicked: {
                // messagBox.forceClose();
            }

        }

        BorderImage {

            id: messageContent

            clip: true
            anchors {
                   left: parent.left;
                   right: parent.right;
                   top: titleBar.bottom;
                   bottom: parent.bottom;
            }

            source: "image://theme/titlebar_l" //ck

            Text {

                id: messageText

                width: parent.width

                anchors {
                    top: parent.top
                    bottom: buttonField.top
                    left: parent.left
                    right: parent.right
                    leftMargin: 5;
                    rightMargin: 5
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter

                wrapMode: Text.WordWrap
                color: theme_dialogTitleFontColor
                font.pixelSize: theme_fontPixelSizeLarge //ck what size it is
            }

            Item {

                id: buttonField

                anchors { left: parent.left;
                          right: parent.right;
                          bottom: parent.bottom;
                }
                height: 100

                property bool showAcceptButton: true
                property bool showRejectButton: false
                property bool showCancelButton: false

                property string acceptButtonText: qsTr("OK")
                property string rejectButtonText: qsTr("Reject")
                property string cancelButtonText: qsTr("Cancel")

                property bool acceptButtonActive: true
                property bool rejectButtonActive: false
                property bool cancelButtonActive: true

                Row {

                    id: buttonRow

                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 5
                    anchors.rightMargin: 5

                    spacing: 5

                    Button {
                        id: cancelButton

                        visible: buttonField.showCancelButton
                        title: buttonField.cancelButtonText
                        active: buttonField.cancelButtonActive
                        enabled: active

                        onClicked: {
                            messageBox.canceled();
                            forceClose();
                        }
                        width: (buttonField.width / 3) - 10

                    }
                    Button {
                        id: rejectButton

                        visible: buttonField.showRejectButton
                        title: buttonField.rejectButtonText
                        active: buttonField.rejectButtonActive
                        enabled: active

                        onClicked: {
                            messageBox.rejected();
                            forceClose();
                        }
                        width: (buttonField.width / 3) - 10
                    }
                    Button {
                        id: acceptButton

                        visible: buttonField.showAcceptButton
                        title: buttonField.acceptButtonText
                        active: buttonField.acceptButtonActive
                        enabled: active

                        onClicked: {
                            messageBox.accepted();
                            forceClose();
                        }
                        width: (buttonField.width / 3) - 10

                    }
                }
            }
        }

        Behavior on opacity {
            NumberAnimation { duration: theme_dialogAnimationDuration }
        }
    }
}


