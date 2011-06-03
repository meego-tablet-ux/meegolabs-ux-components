/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

/* The MainPage lets the user switch between different contents which
   show the widgets available in these components. */

import Qt 4.7
import MeeGo.Components 0.1

AppPage {
    id: mainPage

    state: "common"

    pageTitle: qsTr("Book 1, widget gallery")

    actionMenuModel: [ qsTr("Landscape"),
                       qsTr("Portrait"),
                       qsTr("Inv. Landscape"),
                       qsTr("Inv. Portrait") ]
    // BUG: enums are not called correctly. Commented out for now.
//    actionMenuPayload: [ Scene.landscape , Scene.portrait, Scene.invertedLandscape, Scene.invertedPortrait ]
    actionMenuPayload: [ 1 , 2, 3, 4 ]
    actionMenuTitle: qsTr("Action Menu")

    onActionMenuTriggered: {

        if( selectedItem == 1) {
            window.orientation = 1
        } else if( selectedItem == 2) {
            window.orientation = 2
        } else if( selectedItem == 3) {
            window.orientation = 3
        } else if( selectedItem == 4) {
            window.orientation = 0
        }
    }

    Item {
        id: contentButtons

        property int buttonWidth: parent.width * 0.2 //200
        property int buttonHeight: 60
        property int buttonMargins: 2

        property string activeButtonImage: "image://themedimage/widgets/common/button/button-default"
        property string buttonImage: "image://themedimage/widgets/common/button/button"
        property string buttonImagePressed: "image://themedimage/widgets/common/button/button-default-pressed"

        width: 3 * buttonWidth + 4 * buttonMargins
        height: buttonHeight
        anchors.top: parent.top
        anchors.topMargin:  10
        anchors.horizontalCenter: parent.horizontalCenter

        Button {
            id: buttonsButton

            width:  parent.buttonWidth; height: parent.buttonHeight
            anchors { margins: parent.buttonMargins; right: settersButton.left }
            text: qsTr("common")
            active: true

            onClicked: {
                mainPage.state = "common"
                active = true
                settersButton.active = false
                pickersButton.active = false
            }
        }

        Button {
            id: settersButton

            width:  parent.buttonWidth; height: parent.buttonHeight
            anchors { margins: parent.buttonMargins; horizontalCenter: parent.horizontalCenter }
            text: qsTr("media")
            enabled: true

            onClicked: {
                mainPage.state = "media"
                active = true
                pickersButton.active = false
                buttonsButton.active = false
            }
        }

        Button {
            id: pickersButton

            width:  parent.buttonWidth; height: parent.buttonHeight
            anchors { margins: parent.buttonMargins; left: settersButton.right }
            text: qsTr("datetime")
            enabled: false

            onClicked: {
                mainPage.state = "datetime"
                active = true
                settersButton.active = false
                buttonsButton.active = false
            }
        }
    }

    Item {
        id: contentSpace

        anchors { top: contentButtons.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
    }

    CommonContent { id: commonContent; anchors.fill: contentSpace }
    MediaContent { id: mediaContent; anchors.fill: contentSpace }
    DateTimeContent { id: datetimeContent; anchors.fill:  contentSpace }

    Rectangle { z: -1; anchors.fill: parent; color: "grey" } //background

    states:  [
        State {
            name: "common"
            PropertyChanges { target: commonContent; visible: true }
            PropertyChanges { target: mediaContent; visible: false }
            PropertyChanges { target: datetimeContent; visible: false }
        },
        State {
            name: "media"
            PropertyChanges { target: commonContent; visible: false }
            PropertyChanges { target: mediaContent; visible: true }
            PropertyChanges { target: datetimeContent; visible: false }
        },
        State {
            name: "datetime"
            PropertyChanges { target: commonContent; visible: false }
            PropertyChanges { target: mediaContent; visible: false }
            PropertyChanges { target: datetimeContent; visible: true }
        }

    ]
}
