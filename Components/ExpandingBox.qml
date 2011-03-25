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
    height: pulldownImage.height
    width: 300
    property int containerHeight: 77
    property int padding: 10
    property int expandedHeight: detailsItem.height
    property bool expanded: false
    property bool containerHeightSet: false
    property Component detailsComponent: null
    property Item detailsItem: null
    property Item expandButton: expandButton1
    property alias titleText: titleText.text //deprecated: use titleTextItem
    property alias titleTextColor: titleText.color //deprecated: use titleTextItem
    property alias titleTextItem: titleText
    property alias pulldownImageSource: pulldownImage.source

    signal changed(bool expanded)

    onDetailsComponentChanged: {
        if(detailsItem) detailsItem.destroy();
        detailsItem = detailsComponent.createObject(boxDetailsArea)
    }

    Component.onCompleted: {
        if(detailsComponent) {
            if (detailsItem) detailsItem.destroy();
            detailsItem = detailsComponent.createObject(boxDetailsArea)
        }
    }

    onExpandedChanged: {
        expandButton1.source = expanded ? "image://theme/settings/pulldown_arrow_up" : "image://theme/settings/pulldown_arrow_dn"
            container.changed(expanded);
    }

    BorderImage {
        id: pulldownImage
        width: parent.width
        border.top: 22
        border.bottom:22
        border.right: 0
        border.left: 0
        horizontalTileMode: BorderImage.Stretch
        verticalTileMode: BorderImage.Stretch
        source: "image://theme/pulldown_box"

        onStatusChanged: {
            if((status == BorderImage.Ready) && (!containerHeightSet)) {
                container.containerHeight = pulldownImage.height
                containerHeightSet = true
            }
        }

        Item {
            id: header
            width: parent.width
            height: 77

            Text {
                id: titleText
                font.pixelSize: theme_fontPixelSizeLargest      //THEME
                color:  theme_fontColorHighlight         //THEME
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: expandButton1.verticalCenter
            }

            Image {
                id: expandButton1
                anchors.right: parent.right
                anchors.rightMargin: 6
                anchors.verticalCenter: parent.verticalCenter
                source: "image://theme/settings/pulldown_arrow_dn"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: expanded = !expanded;
            }
        }

        Item {
            id: boxDetailsArea
            visible: container.expanded
            anchors.top: header.bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.margins: container.padding
        }
    }
    states: [
        State {
            name: "expanded"
            PropertyChanges {
                target: pulldownImage
                height: expandedHeight + containerHeight + padding*2
            }
            PropertyChanges {
                target: boxDetailsArea
                visible: true
                opacity: 1.0
            }

            when: { container.expanded }
        },
        State {
            name: "normal"
            PropertyChanges {
                target: pulldownImage
                height: container.containerHeight
            }
            PropertyChanges {
                target: boxDetailsArea
                visible: false
                opacity: 0
            }

            when: { !container.expanded }
        }
    ]

    transitions: [
        Transition {
           SequentialAnimation {
                NumberAnimation {
                    properties: "height"
                    duration: 200
                    easing.type: Easing.InCubic
                }
                NumberAnimation {
                    properties: "opacity"
                    duration: 1000
                    easing.type: Easing.OutCubic
                }
            }
        }
    ]
}
