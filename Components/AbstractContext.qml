/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Components 0.1

Item {
    id: container
    visible: false

    onVisibleChanged: {
        modalSurface.visible = visible
    }

    Connections {
        target: modalSurface
        onVisibleChanged: {
            visible = modalSurface.visible
        }
    }

    property int radius
    property string color: theme_contextMenuBackgroundColor
    property alias contentWidth: modalSurface.contentWidth
    property alias contentHeight: modalSurface.contentHeight
    property alias contentX: modalSurface.contentX
    property alias contentY: modalSurface.contentY
    property alias targetParent: modalSurface.targetParent

    property double menuOpacity: 1
    property double fogOpacity: 0.4

    property int mouseX: 0
    property int mouseY: 0

    property int fingerX: 0
    property int fingerY: 0

    property variant payload
    property int itemHeight: 50
    property int fingerMode: 0
    property bool forceFingerMode: false

    property Component content: null
    property Item contentItem: null

    property alias modalSurfaceItem: modalSurface

    signal close();

    ModalSurface {
        id: modalSurface

        onClose: { container.close(); }

        content: Item {
            id: everything
            height: childrenRect.height
            width: childrenRect.width


            Connections {
                target: container
                onContentItemChanged: {
                    everything.rescale();
                }
                onMouseXChanged: {
                    everything.rescale()
                }
                onMouseYChanged: {
                    everything.rescale();
                }
            }

            // Clamp val between min and max
            function clamp (val, min, max) {
                return Math.max (min, Math.min (max, val));
            }

            Component.onCompleted: {
                rescale();
            }

            function rescale() {
                var mouseX = container.mouseX == 0 ? container.contentX : container.mouseX
                var mouseY = container.mouseY == 0 ? container.contentY : container.mouseY

                // Ensure that the context menu will completely fit
                // on the screen, and that the menu finger is attatched
                // to the correct corner so its obvious what element
                // the context menu is associated with.
                //
                // menuContainer.fingerMode:
                // 0 - left
                // 1 - right
                // 2 - top
                // 3 - bottom

                var screenPadding = 10
                var fingerSize = 30;
                var fingerOffsetMagic = 15; // The distance of the finger arrow pic's edge to the real arrow point

                var pw = modalSurface.targetParent.width
                var ph = modalSurface.targetParent.height
                var mw = menu.width;
                var mh = menu.height;
                var fmode = 0;

                // Step one
                // find the appropriate direction for the finger to point
                // Horizontal placement takes precedence over vertical.
                if (mouseX + mw > pw) {
                    fmode = 1;
                } else if (mouseX - mw < 0) {
                    fmode = 0;
                }

                // Check vertically
                if (mouseY + (mh / 3) > ph && mouseY - mh > 0 ) {
                    // Switch to bottom
                    fmode = 3;
                }  if (mouseY - (mh / 3) < 0 && (mouseY + mh < ph) ) {
                    // Switch to top
                    fmode = 2;
                }

                if(!container.forceFingerMode) container.fingerMode = fmode;

                // Step two
                // Given the finger direction, find the correct location
                // for the menu, keeping it onscreen.
                switch (container.fingerMode) {
                    case 0:
                    case 1:
                        mouseX -= fingerOffsetMagic;

                        if( mouseY - (mh / 2) < 0 )
                            container.contentY = 0;
                        else if( mouseY + (mh / 2) > ph )
                            container.contentY = ph - mh;
                        else
                            container.contentY = mouseY - (mh / 2);

                        if (container.fingerMode == 0) {
                            container.contentX = mouseX;
                        } else {
                            container.contentX = mouseX - mw;
                        }
                        break;

                    case 2:
                    case 3:
                        // Clamp mouseX so that at the edges of the screen we don't
                        // try putting the finger at a location where the
                        // menu can't be placed.
                        // We don't need to do something similar to mouseY because
                        // when it would happen to mouseY, we have switched to
                        // top or bottom fingermode, and so it becomes a
                        // mouseX issue
    //                    var borderSize = 35;
    //                    mouseX = clamp (mouseX, borderSize, pw - borderSize);


//                        container.contentX = clamp (mouseX - mw / 2, 0, (pw - mw));
                        container.contentX = clamp (mouseX - mw / 2, 0, (pw - mw));

                        if (container.fingerMode == 2) {
                            mouseY = mouseY - fingerSize + fingerOffsetMagic;
                            container.contentY = mouseY;
                        } else {
                            mouseY -= fingerOffsetMagic;
                            container.contentY = mouseY - mh;
                        }
                        break;
                }

                container.fingerX = mouseX - container.contentX;
                container.fingerY = mouseY - container.contentY;
            }

            Item {
                id: menu
                x: 0
                y: 0
                width: realMenu.width
                height: realMenu.height

                property alias contentWidth: realMenu.width

                opacity: container.menuOpacity

                Column {
                    id: realMenu

                    x: if (container.fingerMode == 0) {
                        return finger.width - 2;
                    } else {
                        return 0;
                    }
                    y: if (container.fingerMode == 2) {
                        return finger.height - 2;
                    } else {
                        return 0;
                    }
                    width: backgroundImage.width
                    //height: borderImage1.height + contentArea.height + borderImage2.height

                    BorderImage {
                        id: borderImage1
                        //width: container.contentWidth
                        width: backgroundImage.width
                        border.left: 10
                        border.right: 10
                        source: "image://theme/popupbox_1"

                    }


                    //Temporarily replace BorderImage with regular image,
                    //as BorderImage is causing weird visual glitches on startup,
                    //and the asset we have for the background isn't really
                    //something we need to have a BorderImage for...
//                    BorderImage {
                    Image {
                        id: backgroundImage
                        //width: container.contentWidth
                        width: contentArea.width
                        //border.left: 10
                        //border.right: 10
                        anchors.left: borderImage1.left
                        source: "image://theme/popupbox_2"
                        //verticalTileMode: BorderImage.Repeat
                        //clip: true
                        height: contentArea.height

                        Item {
                            id: contentArea
                            //x: 16
                            width: childrenRect.width
                            height: childrenRect.height

                            Component.onCompleted: {
                                 if(container.content != null) {
                                     container.contentItem = container.content.createObject(contentArea)
                                     //console.log("item width: " + contentArea.width)
                                     if(contentArea.width == 0) container.contentWidth = 300
                                     if(container.contentWidth != 0 ) {
                                         //console.log("manually setting width of item")
                                         contentArea.width = container.contentWidth
                                         everything.rescale();
                                     }

                                     if(contentArea.height == 0) container.contentHeight = 200
                                     if(container.contentHeight != 0 || contentArea.height == 0) {
                                         //console.log("manually setting height of item")
                                         contentArea.height = container.contentHeight
                                         everything.rescale();
                                     }
                                 }
                            }

                            onWidthChanged: {
                                everything.rescale();
                            }
                            onHeightChanged: {
                                everything.rescale();
                            }
                        }
                    }

                    BorderImage {
                        id: borderImage2
                       // width: container.contentWidth
                        width: backgroundImage.width
                        border.left: 10
                        border.right: 10

                        source: "image://theme/popupbox_3"
                    }
                }

                Behavior on opacity {
                    PropertyAnimation { duration: 500 }
                }

                Image {
                    id: finger

                    x: if (container.fingerMode == 0) {
                        return fingerX ;
                    } else if (container.fingerMode == 1) {
                        return fingerX;
                    } else {
                        return fingerX - (width / 2);
                    }

                    y: if (container.fingerMode == 0 || container.fingerMode == 1) {
                        return fingerY - (height / 2);
                    } else {
                        return fingerY;
                    }

                   // width: 30   // fixed arrow image file size
                   // height: 30

                    smooth: true

                    Behavior on opacity {
                        PropertyAnimation { duration: 100 }
                    }
                }

                states: [
                    State {
                        name: "left"
                        when: container.fingerMode == 0
                        PropertyChanges {
                            target: finger
                            source: "image://theme/popupbox_arrow_left"
                        }
                    },
                    State {
                        name: "right"
                        when: container.fingerMode == 1
                        PropertyChanges {
                            target: finger
                            source: "image://theme/popupbox_arrow_right"
                        }
                    },
                    State {
                        name: "top"
                        when: container.fingerMode == 2
                        PropertyChanges {
                            target: finger
                            source: "image://theme/popupbox_arrow_top"
                        }
                    },
                    State {
                        name: "bottom"
                        when: container.fingerMode == 3
                        PropertyChanges {
                            target: finger
                            source: "image://theme/popupbox_arrow_bottom"
                        }
                    }
                ]
            }
        }
    }
}
