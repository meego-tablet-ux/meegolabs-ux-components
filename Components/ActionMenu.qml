/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Components 0.1

Flickable {
    id: container

    property alias model: repeater.model

    property int minWidth : 200     // The width of the menu should always be between minWidth and maxWidth
    property int maxWidth : 500
    property int currentWidth: minWidth
    property int highlightIndex : -1
    property int textMargin : 16
    property string title : ""

    property bool elideEnabled: false

    property bool flickMoving: false


    signal triggered(int index)
    signal moveIt()

    // currentWidth is the current width of the largest text width, clamped between minWidth and maxWidth
    width: currentWidth
    height: layout.height

    contentHeight: layout.height
    contentWidth: currentWidth

    interactive: false
    clip: true

    Column {
        id: layout

        width: parent.width

        Text {
            id: titleItem
            color: theme_fontColorHighlight
            font.pixelSize: theme_contextMenuFontPixelSize
            visible: (title.length != 0)
            text: title
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: currentWidth
        }

        Image {
            id: titleSeperatorImage
            anchors.top: titleItem.bottom
            anchors.topMargin: 5
            width: currentWidth
            visible: (title.length != 0)
            source: "image://theme/menu_item_separator"
        }

        Repeater {
            id: repeater

            width: parent.width
            height: childrenRect.height

            delegate: Item {
                id: delegateThingy

                width: parent.width
                height: textItem.paintedHeight + textMargin*2

                clip : true

                Text {
                    id: textItem

                    x: textMargin

                    width: parent.width - textMargin * 2
                    height: delegateThingy.height - 1   // -1 to leave space for the seperator

                    verticalAlignment: Text.AlignVCenter

                    color:(highlightIndex == index)?theme_fontColorHighlight:theme_contextMenuFontColor
                    font.pixelSize: theme_contextMenuFontPixelSize

                    // elide has to be turned of to correctly compute the paintedWidth. It is re-enabled after the width computing
                    elide: if(elideEnabled){ Text.ElideRight; }else{ Text.ElideNone; }

                    text: modelData

                    Component.onCompleted: {
                        // This compares the paintedWidth to minWidth and maxWidth and sets the widgets width accordingly
                        if( paintedWidth + textMargin * 2  > container.currentWidth ){
                            if( paintedWidth + textMargin * 2 > maxWidth )
                                container.currentWidth = maxWidth;
                            else{
                                container.currentWidth = paintedWidth + textMargin * 2;
                            }
                        }
                        if( currentWidth > maxWidth )
                            currentWidth = maxWidth;
                    }
                }

                Image {
                    id: seperatorImage

                    anchors.top: textItem.bottom
                    width: currentWidth

                    visible: index < repeater.count - 1     // Seperator won't be visible for the last item

                    source: "image://theme/menu_item_separator"
                }

                MouseArea {
                    anchors.fill: parent

                    // pressed state for the text entry:
                    onClicked: {
                        container.triggered(index)
//                        textItem.opacity = 1
                    }
                    onPressed: {
//                        textItem.opacity = 0.5
                    }
                    onExited: {
//                        textItem.opacity = 1
                    }
                }
            }

            // enable text eliding when the correct width was computed
            Component.onCompleted: {
                elideEnabled = true;
            }
        }
    }

    TopItem {
        id: topItem
    }

    states: [
        State {
           PropertyChanges {
                target: container
                height: topItem.topItem.height - 100
                interactive: true
            }
           when: layout.height > topItem.topItem.height - 100
        }
    ]
}
