/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Item {
    id: checkBox
    height: 30
    width: 30
    property bool isChecked:false
    Image {
        id: checkIcon
        source: checkBox.isChecked?"image://theme/btn_tickbox_dn":"image://theme/btn_tickbox_up"
        anchors.fill: parent
        MouseArea {
            anchors.fill: parent
            onClicked: {
                checkBox.isChecked = !checkBox.isChecked;
            }
        }

    }
}//end of checkbox
