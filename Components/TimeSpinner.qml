/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Rectangle {
    id: outer

    property int min: 1
    property int incr: 1
    property int count: 12
    property int value: (tsview.currentIndex * incr) + min
    property bool pad: false


    function displayValue(index) {
        var n = (index * incr) + min

        if (pad && n < 10)
            return "0" + n
        else
            return n
    }

    function setValue(v) {
        tsview.currentIndex = (value - min) / incr
    }

    Component {
        id: tsdelegate

        Text {
            id: delegateText
            font.pixelSize: theme_fontPixelSizeSmall
            text: displayValue(index)
            height: font.pixelSize + 10
            color: "#A0A0A0"

            states: [
                State {
                    name: "active"
                    when:  text == (tsview.currentIndex * incr) + min
                    PropertyChanges { target: delegateText; font.bold: true; color: theme_fontColorNormal; font.pixelSize: theme_fontPixelSizeLargest3;height:60}
                }
            ]
        }
    }

    Item {
        id: timespinner
        property int numberOfItems: count
        property real itemHeight: 30

        clip: true
        anchors.fill: parent
        focus: true
        BorderImage {
            id: bgImage
            source: "image://theme/pickers/timespinbg"
            anchors.fill: parent
            opacity: 0.5
        }

        PathView {
            id: tsview
            anchors.fill: timespinner
            BorderImage {
                id: innerBgImage
                source:"image://theme/pickers/timespinhi"
                anchors.fill: parent
                opacity:0.25
            }

            model: timespinner.numberOfItems
            delegate: tsdelegate
            path:  Path {
                startX: tsview.width / 2
                startY: 5
                PathLine {
                    x: tsview.width / 2
                    y: tsview.pathItemCount * timespinner.itemHeight + timespinner.itemHeight
                }               
            }

            pathItemCount: 3
            preferredHighlightBegin: 0.47
            preferredHighlightEnd: 0.47

            dragMargin: tsview.width/2
        }
    }
}
