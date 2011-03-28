/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

BorderImage {
    id: container

    property alias text: input.text
    property alias defaultText: fakeText.text
    property alias textInput: input
    property alias inputMethodHints: input.inputMethodHints

    border.top: 10
    border.bottom: 10
    border.left: 10
    border.right: 10

    height: 50
    source: "image://theme/email/frm_textfield_l"

    signal textChanged

    Text {
        id: fakeText
        x: 15
        y: 15
        anchors {top:  parent.top; topMargin: 13 }

        font.pixelSize: theme_fontPixelSizeLarge
        color: "slategrey"

        visible: (input.text == "")

        Connections {
            target: input
            onTextChanged: {
                fakeText.visible = (input.text == "")
            }
        }

    }

    TextInput {
        id: input

        x: 15
        y: 15
        width: parent.width - 30
        anchors {top:  parent.top; topMargin: 13 }

        font.pixelSize: theme_fontPixelSizeLarge

        // TODO: Add this capability back once it has been proven
        //       to work across the entire stack
        CCPContextArea {
            editor: parent
        }

        onTextChanged: {
            container.textChanged ();
        }

        Keys.onReturnPressed: {
            // Focus out when return is pressed
            input.focus = false
        }
        onActiveFocusChanged: {
            // No focus on input field, hide VKB panel
            if(!activeFocus) input.closeSoftwareInputPanel();
        }
    }

    states: [
        State {
            name: "Enabled"
            PropertyChanges {
                target: container
                opacity: 1.0
            }
            when: container.enabled
        },
        State {
            name: "Disabled"
            PropertyChanges {
                target: container
                opacity: 0.5
            }
            when: !container.enabled
        }
    ]

}
