/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

ContextMenu {
    model: [qsTr ("Copy"), qsTr ("Cut"), qsTr ("Paste")]

    onTriggered: {
        switch (index) {
        case 0:
            payload.copy ();
            break;

        case 1:
            payload.cut ();
            break;

        case 2:
            payload.paste ();
            break;

        default:
            break;
        }
    }
}
