/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Media 0.1


/* Modal Message Box
 * This is a basic template for message boxes
 */

Fog {
    id: modalMessageBox

//    property bool albumSelectionMode: false
    property string promptText: ""//albumSelectionMode ? qsTr("Pick an album") : qsTr("Pick a photo")
//    property string albumText: qsTr("Albums")

    property int boxWidth: 600//: inner.width
    property int boxHeight: 300//: inner.height

    property int contentsMargins: 10

    property alias content: innerContentArea.children //bob.children

//    property Item innerItem//: null
//    property Item innerContentItem//: null

    property Component innerContent//: Rectangle{ width: 20; height: 20; color:"red"; visible: true }//: null

    signal cancel

//    // probably will remove these signals at some point
    signal closed
    signal opened

    signal close

    autoCenter: true

//    function show() {
//        console.log("DEPRECATED! Call visible = true on the PhotoPicker yourself!")
//        visible = true
//    }

    onVisibleChanged: {
        if(!visible) {
            close();
        }
//            else {

//                console.log("activating modal surface")

//                if(modalMessageBox.innerItem) modalMessageBox.innerItem.destroy();

//                if(modalMessageBox.innerContent == null) {
//                    console.log("ModalSurface cannot create content for something null")
//                    return;
//                }

//                if (modalMessageBox.targetParent == null) {
//                    modalMessageBox.targetParent = topItem.topItem;
//                }
//                modalMessageBox.innerContentItem = innerContent.createObject (innerContentArea);
//            }
    }

    onClose: {
        if(visible) visible = false
    }

    modalSurface: BorderImage {
        id: inner

        width: boxWidth
        height: boxHeight

        property alias innerContentAreaChild: innerContentArea

        TopItem {
            id: topItem
        }

        border.top: 14
        border.left: 20
        border.right: 20
        border.bottom: 20

        source: "image://theme/notificationBox_bg"

        Text {
            id: header

            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            text: photoPicker.promptText
            font.weight: Font.Bold
            font.pixelSize: theme_dialogTitleFontPixelSize
            color: theme_dialogTitleFontColor
        }

        Item {
            id: innerContentArea
            anchors { left: parent.left; top: header.bottom; right: parent.right; bottom: footer.top }

            anchors.margins: contentsMargins
        }

        Row {
            id: footer
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            height: childrenRect.height
            spacing: 18
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                id: button1
                width: 210
                height: 60
                title: qsTr("OK")
                onClicked: {
                    modalMessageBox.closed()
                }
            }

            Button {
                id: button2
                width: button1.width
                height: button1.height
                title: qsTr("Cancel")
                onClicked: {
                    modalMessageBox.cancel()
                }
            }
        }
    }
}
