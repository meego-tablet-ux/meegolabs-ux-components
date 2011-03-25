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

    property variant liveWindowModel
    property alias recentsWindowModel: gridView.model

    signal closeDialog
    signal raisePanel
    signal launch(string filename)
    signal raise(int windowId)
    signal close(int windowId)

    MouseArea {
        anchors.fill: parent
        onClicked:  container.closeDialog()
    }

    BorderImage {
        source: "image://theme/notificationBox_bg"
        anchors.fill: parent
        anchors.margins: 20
        border.left: 50
        border.top: 50
        border.right: 50
        border.bottom: 50
    }

    Item {
        id: innerContainer
        anchors.fill: parent
        anchors.margins: 50

        Item {
            id: panelsButton
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 10

            width: 100
            height: panelsButtonIcon.height + panelsButtonLabel.height + 10

            Image {
                id: panelsButtonIcon
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                source: "image://systemicon/meego-ux-panels"
            }
            Text {
                id: panelsButtonLabel
                anchors.bottom: parent.bottom
                width: parent.width
                height: 20
                color: theme_buttonFontColor
                text: qsTr("Panels")
                font.pixelSize: theme_fontPixelSizeMedium
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    container.closeDialog();
                    container.raisePanel();
                }
            }
        }

        Rectangle {
            id: seperator
            anchors.top:  panelsButton.bottom
            anchors.topMargin: 10
            width: parent.width
            height: 1
            color: "slategray"
        }

        GridView {
            id: gridView
            anchors.top: seperator.bottom
            anchors.topMargin: 10
            width: parent.width
            height: parent.height - y
            clip: true
            cellWidth: width/4 - 1
            cellHeight: 140
            property int itemWidth: cellWidth - 20
            property int itemHeight: cellHeight - 20
            header: Item {
                width: gridView.width
                height: liveWindowRepeater.model.size() > 0 ? gridView.cellHeight : 0
                Row {
                    anchors.fill: parent
                    Repeater {
                        id: liveWindowRepeater
                        model: liveWindowModel
                        Item {
                            id: dinstance
                            width: gridView.cellWidth
                            height: gridView.cellHeight
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    container.closeDialog();
                                    container.raise(windowId);
                                }
                            }
                            Item {
                                id: iconContainer
                                anchors.top: parent.top
                                width: dinstance.width
                                height: dinstance.height - 20
                                Image {
                                    anchors.centerIn: parent
                                    width: 100
                                    height:  100
                                    fillMode: Image.PreserveAspectFit
                                    source: icon
                                }
                            }

                            Item {
                                anchors.right: parent.right
                                anchors.top: parent.top
                                width: 20
                                height: 20
                                Image {
                                    anchors.centerIn: parent
                                    source: "images/close.png"
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        container.close(windowId);
                                    }
                                }
                            }
                            Text {
                                anchors.top: iconContainer.bottom
                                width: dinstance.width
                                height: 20
                                color: theme_fontColorHighlight
                                elide: Text.ElideRight
                                text: title
                                font.pixelSize: theme_fontPixelSizeMedium
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                }
            }
            delegate: Item {
                id: dinstance
                width: gridView.itemWidth
                height: gridView.itemHeight

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        container.closeDialog();
                        container.launch(filename);
                    }
                }

                Item {
                    id: iconContainer
                    anchors.top: parent.top
                    width: dinstance.width
                    height: dinstance.height - 20
                    Image {
                        anchors.centerIn: parent
                        width: 100
                        height:  100
                        fillMode: Image.PreserveAspectFit
                        source: icon
                    }
                }

                Item {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    width: 20
                    height: 20
                    Image {
                        anchors.centerIn: parent
                        source: "images/close.png"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: gridView.model.remove(filename)
                    }
                }
                Text {
                    anchors.top: iconContainer.bottom
                    width: dinstance.width
                    height: 20
                    color: theme_fontColorHighlight
                    elide: Text.ElideRight
                    text: title
                    font.pixelSize: theme_fontPixelSizeMedium
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}
