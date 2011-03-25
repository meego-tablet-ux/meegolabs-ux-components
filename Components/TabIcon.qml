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
    clip:true
    width: 145
    height: 112
    property int radius: 10
    property color color: "Silver"
    property color highlightcolor: "gray"
    property bool highlighted: false
    property int margin: 10
    property alias text: containerText.text
    property int payload: -1
    property int contentHeight: icon.height
    property int contentWidth: icon.width

    signal triggered(int index)

    function deselect() {
        if (highlighted) {
            highlighted = false;
        }
    }

    function select() {
        if (highlighted) {
            return;
        }else {
            highlighted = true;
        }
    }

    BorderImage {
        id: icon
        anchors.fill: parent
        anchors.leftMargin: {
            if (highlighted)
                -1;
            else if (index == 0)
                1;
            else
                0;
        }
        border.bottom: 30
        border.top: 25
        border.left: 20
        border.right: 20
        source: {
            if (index == 0)
            {
                container.highlighted ? "image://theme/tab_top_dn" : "image://theme/tab_top_up";
            }
            else
            {
                container.highlighted ? "image://theme/tab_mid_dn" : "image://theme/tab_mid_up";
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                container.triggered(container.payload);
            }
        }
    }
    Text {
        id: containerText
        anchors.fill: parent
        anchors.bottomMargin: {
            if (index == 0)
            {
                font.pixelSize/2;
            }
            else
            {
                0;
            }
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: theme_fontPixelSizeLargest
        color: theme_fontColorHighlight
        elide:Text.ElideRight
    }
}
