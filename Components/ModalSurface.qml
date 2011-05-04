/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import Qt.labs.gestures 2.0
import MeeGo.Components 0.1

Item {
    id: container
    visible: false

    property int radius
    property int color
    property int contentX: 0
    property int contentY: 0
    property int contentWidth: 0
    property int contentHeight: 0

    property Item targetParent: null

    property double fogOpacity: 0.4

    signal close()

    property Item item: null
    property Item contentItem: null

    property Component content: null

    property bool autoCenter: false
    property bool closeUponFogClicked: true

    TopItem {
        id: topItem
    }

    onVisibleChanged: {
        if(!visible) {
            close();
        }
        else {

            if(container.item) container.item.destroy();

            if(content == null) {
                console.log("ModalSurface cannot create content for something null")
                return;
            }

            if (container.targetParent == null) {
                container.targetParent = topItem.topItem;
            }
            container.item = contextVisualLogic.createObject (container.targetParent);
        }
    }

    onClose: {
        if(visible) visible = false
        container.item.destroy()
    }

    Component {
        id: contextVisualLogic
        Item {
            id: everything
            anchors.fill: parent

            property Component content: container.content

            Component.onCompleted: {
                if(content != null) container.contentItem = content.createObject(contentArea)
            }

            Rectangle {
                id: fog
                anchors.fill: parent
                color: theme_dialogFogColor
                opacity: container.fogOpacity
                Behavior on opacity {
                    PropertyAnimation { duration: 500 }
                }
            }

            GestureArea {
              anchors.fill: parent
              Tap {}
              TapAndHold {}
              Pan {}
              Pinch {}
              Swipe {}
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    if (container.closeUponFogClicked) {
                        container.visible = false;
                    }
                }
            }

            Item {
                id: contentArea
                width: childrenRect.width
                height: childrenRect.height

                Component.onCompleted: {
                    if (autoCenter) {
                        anchors.centerIn = parent
                    }
                    else {
                        x = container.contentX
                        y = container.contentY
                    }
                }

		onWidthChanged: {
			if (autoCenter) {
			    anchors.centerIn = parent
			}
		}
		onHeightChanged: {
			if (autoCenter) {
			    anchors.centerIn = parent
			}
		}
            }

        }
    }
}
