/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Item {
    id: root
    width: image.width
    height: image.height

    property bool checked:false
    property variant value
    property QtObject group

    function click(mouse) { mouseArea.clicked(mouse); }

    Image {
        id: image
        width: sourceSize.width
        height: sourceSize.height
        source: root.checked?"image://theme/btn_radio_dn":"image://theme/btn_radio_up"
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                if (!checked && group) { group.check(root); }
            }
        }
    }

    onGroupChanged: {
        if (group) { group.add(root); }
    }
}//end of radiobutton
