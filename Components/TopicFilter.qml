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

    property int topicHeight: 35
    property alias model: listView.model
    property variant topics: []
    onTopicsChanged: {
        listModel.clear()
        for (var i=0; i < topics.length; i++)
            listModel.append({"mitem": topics[i], "mindex": i})
    }
    signal topicTriggered(int index)
    property alias currentTopic: listView.currentIndex
    property alias interactive: listView.interactive

    ListView {
        id: listView
        anchors.fill: parent

        onCurrentIndexChanged: container.topicTriggered(currentIndex)

        model: ListModel { id: listModel }
        highlight: Rectangle {
            width: listView.width;
            height: container.topicHeight;
            color: "#281832"
        }
        highlightMoveDuration: 1
        delegate: Item {
            id: contentItem
            width: container.width
            height: container.topicHeight

            property int index: mindex

            Image {
                anchors.fill: parent
                source: "images/filter-background.png"
            }

            Image {
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                source: "images/right-arrow.png"
            }

            Text {
                id: contentLabel
                height: container.topicHeight
                width: container.width
                text: mitem
                font.pixelSize: theme_fontPixelSizeMedium
                color: theme_fontColorHighlight
                anchors.left: parent.left
                anchors.leftMargin: 10
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listView.currentIndex = index
                }
            }
        }
    }
}
