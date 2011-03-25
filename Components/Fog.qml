/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Item {

    id : fogContainer

    // API
    property alias closeOnFogClick: fog.closeOnFogClick
    property alias fogShow : fog.show
    property alias modalSurface : modalSurface.children // content
    property bool autoCenter: false

    signal fogClicked;
    signal opened
    signal closed
    signal close

    function show() {
        console.log("DEPRECATED! Call visible = true on the PhotoPicker yourself!")
        fog.show = true;
        fogContainer.opened();
    }

    function forceClose() {
        // DEPRECATED
        fog.show = false;
        fogContainer.close();
    }

    // Private
//    anchors.fill: parent
    visible: fog.opacity > 0
    z: 1

    TopItem {
        id: topItem
    }

    onVisibleChanged: {
         if(!visible) {
             close();
             fog.show = false;
         }
         else {
             fog.show = true;
             fogContainer.opened();
//             if (fogContainer.targetParent == null) {
                 fogContainer.parent = topItem.topItem;
//             }
         }
    }

    Rectangle {

        id: fog

        property bool show: false
        property bool closeOnFogClick: true

        z: 1
        x: parent.x
        y: parent.y

        parent: topItem.topItem
        anchors.fill: topItem.topItem
//        anchors.centerIn: topItem.topItem

        width: parent.width
        height: parent.height

        color: theme_dialogFogColor
        opacity: fog.show ? theme_dialogFogOpacity : 0

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if( fogContainer.closeOnFogClick ) {   
                    fogContainer.forceClose();
                }
                fogContainer.fogClicked();
            }
        }

        Behavior on opacity {
            PropertyAnimation { duration: theme_dialogAnimationDuration }
        }
    }

    Item {

        id: modalSurface
        z: 2

        parent: topItem.topItem
//        anchors.centerIn: parent
        width: childrenRect.width
        height: childrenRect.height

        Component.onCompleted: {
            if (autoCenter) {
                anchors.centerIn = fog
            }
            else {
                x = container.contentX
                y = container.contentY
            }
        }

        opacity: fog.show ? 1 : 0

        Behavior on opacity {
            PropertyAnimation { duration: theme_dialogAnimationDuration }
        }

    }
}
