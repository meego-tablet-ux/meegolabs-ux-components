/* Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Flickable {
    id: container

    property alias model: repeater.model

    property int minWidth: 200
    property int maxWidth: 500
    property int currentWidth: minWidth
    property int textMargin: 16

    property bool elideEnabled: false
    property bool flickMoving: false
    property Component delegateComponent: null

    property int currentIndex: 0

    signal triggered (int index, variant data)
    signal moveIt ()

    width: currentWidth
    height: layout.height

    contentHeight: layout.height
    contentWidth: currentWidth

    interactive: false
    clip: true

    Column {
        id: layout

        width: parent.width
        Repeater {
            id: repeater

            width: parent.width
            height: childrenRect.height

            delegate: Item {
                id: menuEntry

                width: parent.width
                height: itemHolder.height + separatorImage.height

                Image {
                    id: indicator

                    x: textMargin

                    anchors.verticalCenter: parent.verticalCenter
                    source: "image://theme/dropdown_indicator"

                    visible: (index == container.currentIndex)
                }

                Item {
                    id: itemHolder

                    property Item delegateItem: null

                    x: indicator.width + 5
                    width: parent.width - indicator.width

                    Component.onCompleted: {
                        console.log ("Component completed");
                        if (container.delegateComponent) {
                            var element = container.delegateComponent.createObject (itemHolder);
                            console.log ("Creating new item");
                            // Fun with QML: If you pass a list object
                            // for the model then what you want is stored
                            // in the delegate variable modelData. If
                            // you pass a model object for the model, then
                            // what you want is in the variable model
                            if (dataModel) {
                                element.data = model;
                            } else {
                                element.data = modelData;
                            }
                            itemHolder.height = element.height;

                            var potentialWidth;

                            potentialWidth = element.width + (textMargin * 2) + indicator.width + 5;

                            if (potentialWidth > container.currentWidth) {
                                if (potentialWidth > maxWidth) {
                                    container.currentWidth = maxWidth;
                                } else {
                                    container.currentWidth = potentialWidth;
                                }
                            }

                            if (currentWidth > maxWidth) {
                                currentWidth = maxWidth;
                            }

                            delegateItem = element;
                        }
                    }
                }

                Image {
                    id: separatorImage

                    anchors.top: itemHolder.bottom
                    width: currentWidth

                    visible: index < repeater.count - 1

                    source: "image://theme/menu_item_separator"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var selval;
                        if (itemHolder.delegateItem != null) {
                            selval = itemHolder.delegateItem.data;
//                            listmodel.currentIndex = index;

                            container.triggered (index, selval);
                            container.visible = false;
                        }
                    }
                }
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
