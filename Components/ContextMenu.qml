/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

AbstractContext {
    id: container

    property variant model: []
    property variant payload

    signal triggered(int index, variant payload)

    property alias mouseX: container.mouseX
    property alias mouseY: container.mouseY
    property int minMenuWidth: 200
    property int maxMenuWidth: 500

    function displayContextMenu (mouseX, mouseY) {
        container.mouseX = mouseX
        container.mouseY = mouseY
        visible = true;
    }

    ///Deprecated:
    property alias menuX: container.mouseX
    property alias menuY: container.mouseY
    property int menuWidth
    property int menuHeight

    content: ActionMenu {
        model: container.model
        minWidth: minMenuWidth
        maxWidth: maxMenuWidth
        onTriggered: {
            container.triggered(index,payload)
            container.visible = false;
        }
    }
}
