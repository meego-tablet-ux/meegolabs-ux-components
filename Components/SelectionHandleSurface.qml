/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import Qt.labs.gestures 2.0

Item {
    id: container
    visible: false

    property alias startHandle: startHandle
    property alias endHandle: endHandle

    property Item editor: null

    signal close()

    TopItem {
        id: topItem

        Item {
            id: innerItem

            visible: false
            parent: topItem.topItem

            anchors.fill: parent

            Image {
                id: startHandle
                source: "image://theme/text-selection-marker-start"

                // mh is unused here
                function setPosition (mx, my, mh) {
                    x = mx - (width / 2);
                    y = my;
                }
            }

            Image {
                id: endHandle
                source: "image://theme/text-selection-marker-end"

                function setPosition (mx, my, mh) {
                    x = mx - (width / 2);
                    y = (my + mh) - height;
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

                property Item selectedHandle: null

                property int xOffset: 0
                property int yOffset: 0
                property bool ignoreRelease: false
                property bool ignorePressAndHold: true

                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton

                function insideHandle (h, mx, my) {
                    return (mx >= h.x && mx <= h.x + h.width) && (my >= h.y && my <= h.y + h.height);
                }

                onPressed: {
                    ignorePressAndHold = true;
                    // We store the handle that the mouse pointer was over
                    // so we can drag the correct end of the selection
                    // on movement.
                    if (insideHandle (startHandle, mouse.x, mouse.y)) {
                        selectedHandle = startHandle;
                    } else if (insideHandle (endHandle, mouse.x, mouse.y)) {
                        selectedHandle = endHandle;
                    } else {
                        ignorePressAndHold = false;
                        selectedHandle = null;
                    }

                    if (selectedHandle) {
                        // Store the offset so that we can stick the handle to
                        // the mouse pointer to get the correct positioning
                        xOffset = mouse.x - selectedHandle.x;
                        yOffset = mouse.y - selectedHandle.y;
                    }
                }

                onReleased: {
                    if (ignoreRelease) {
                        ignoreRelease = false;
                        return;
                    }

                    if (selectedHandle == null) {
                        container.close ();
                    } else {
                        container.editor.showContextMenu (mouse.x, mouse.y);
                    }
                }

                onPressAndHold: {
                    // Ignore press and hold if a click on the handle
                    // initiated this signal so that we don't pop up the
                    // menu in the middle of moving the handle
                    if (ignorePressAndHold) {
                        return;
                    }

                    var map = mapToItem (scene.content, mouse.x, mouse.y);
                    container.editor.showContextMenu (map.x, map.y);

                    // Now a menu is popped up we don't want the release to
                    // close the selection
                    ignoreRelease = true;
                }

                onPositionChanged: {
                    if (selectedHandle) {
                        var nx, ny;

                        // Work out where the handle would be moved to
                        nx = mouse.x - xOffset;
                        ny = mouse.y - yOffset;

                        if (selectedHandle == startHandle) {
                            // Convert the handle's position to the
                            // position of the selection
                            var sx = nx + (selectedHandle.width / 2);
                            var sy = ny;

                            var map = mapToItem (container.editor, sx, sy);

                            // Set the start of the selection
                            // the selection will then move the handle to
                            // the correct place.
                            container.editor.setStartPosition (map.x, map.y);
                        } else {
                            // Same explanation as above, but at the end of
                            // the selection
                            var sx = nx + (selectedHandle.width / 2);
                            var sy = ny + (selectedHandle.height);

                            var map = mapToItem (container.editor, sx, sy);
                            container.editor.setEndPosition (map.x, map.y);
                        }
                    }
                }
            }

       }
    }

    onVisibleChanged: {
        innerItem.visible = visible;
    }

    onClose: {
        if (visible) visible = false
    }
}
