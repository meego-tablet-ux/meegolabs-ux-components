/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Flickable {
    id: flick

    property alias text: editPane.text
    property alias defaultText: fakeText.text

    contentWidth: editPane.paintedWidth
    contentHeight: editPane.paintedHeight
    interactive: true
    flickableDirection: Flickable.VerticalFlick
    clip: true

    function ensureVisible (r) {
        if (contentX >= r.x) {
            contentX = r.x;
        } else if (contentX + width <= r.x + r.width) {
            contentX = r.x + r.width - width;
        }

        if (contentY >= r.y) {
            contentY = r.y;
        } else if (contentY + height <= r.y + r.height) {
            contentY = r.y + r.height - height;
        }
    }

    Text {
        id: fakeText
        x: 10
        y: 10

        font.pixelSize: theme_fontPixelSizeLarge
        color: "slategrey"

        visible: editPane.text == ""
    }

    TextEdit {
        id: editPane
        x: 10
        y: 10
        width: flick.width - 20
        height: flick.height - 20
        wrapMode: TextEdit.Wrap
        font.pixelSize: theme_fontPixelSizeLarge
        focus: true

        onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)

        CCPContextArea {
/*
            // The mouse area needs to expand outside of the flick
            // so that the selection handles can be clicked when they
            // are in the gutter
            x: -10
            y: -10
            width: parent.width + 20
            height: parent.height + 20
*/
            editor: parent
            xOffset: flick.contentX
            yOffset: flick.contentY
        }
    }
}
