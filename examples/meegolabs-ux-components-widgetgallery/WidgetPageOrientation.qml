/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

/* This pages can call up the ExpandingBox.qml and offers controls to manipulate it.
 */

import Qt 4.7
import MeeGo.Components 0.1

WidgetPage {
    id: widgetPage

    pageTitle: qsTr( "Orientation Test" )

    controlContent: [

        LabeledEntry {
            id: widthEntry

            label:  qsTr("orientaion: ")
            defaultValue: window.orientationString

        }

    ]

    description: qsTr("Here you can test the orientation and locks.")

    widget: Flickable {

        anchors.fill: parent
        contentHeight: height > buttonBox.height ? height : buttonBox.height
        contentWidth: width > buttonBox.width ? width : buttonBox.width
        clip: true

        Row {
            anchors.centerIn: parent

            spacing: 25

            Column {

                id: buttonBox

                CheckBoxEntry {
                    id: ebox1

                    label: window.lockCurrentOrientation ? qsTr("unlock Current orientation") : qsTr("lock Current orientation")
                    isChecked: window.lockCurrentOrientation

                    onCheckedChanged: {
                        window.lockCurrentOrientation = !window.lockCurrentOrientation
                    }

                    width: 400

                }
                Button {
                    id: ebox2

                    enabled: !window.isOrientationLocked
                    text: qsTr("set to landscape")

                    width: 450
                    onClicked: {
                        window.orientation = 1 // Scene.landscape
                    }
                }
                Button {
                    id: ebox3

                    enabled: !window.isOrientationLocked
                    text: qsTr("set to inverted landscape")

                    width: 450
                    onClicked: {
                        window.orientation = 3 // Scene.
                    }
                }
                Button {
                    id: ebox4

                    enabled: !window.isOrientationLocked
                    text: qsTr("set to portrait")
                    width: 450
                    onClicked: {
                       window.orientation = 2 // Scene.
                    }
                }
                Button {
                    id: ebox5

                    enabled: !window.isOrientationLocked
                    text: qsTr("set to inverted portrait")
                    width: 450
                    onClicked: {
                        window.orientation = 0 // Scene.invertedPortrait
                    }
                }
            }

            Column{
                id: radioBoxex

                spacing: 10

                RadioGroup {
                    id: orientationLocks

                    onSelectedValueChanged: {
                        window.orientationLock = orientationLocks.selectedValue
                    }
                }
                RadioButton {
                    id: button1

                    group: orientationLocks

                    text: "no Lock"
                    value: 0 //Scene.noLock
                }
                RadioButton {
                    id: button2

                    group: orientationLocks

                    text: "Lock to landscape"
                    value: 1 //Scene.lockLandscape
                }
                RadioButton {
                    id: button3

                    group: orientationLocks

                    text: "Lock to portrait"
                    value: 2 //Scene.lockPortrait
                }
                RadioButton {
                    id: button4

                    group: orientationLocks

                    text: "Lock to inverted landscape"
                    value: 3 //Scene.lockInvertedLandscape
                }
                RadioButton {
                    id: button5

                    group: orientationLocks

                    text: "Lock to inverted Portrait"
                    value: 4 //Scene.lockInvertedPortrait
                }
                RadioButton {
                    id: button6

                    group: orientationLocks

                    text: "Lock to both Landscapes"
                    value: 5 //Scene.lockAllLandscape
                }
                RadioButton {
                    id: button7

                    group: orientationLocks

                    text: "Lock to both Portraits"
                    value: 6 //Scene.lockAllPortrait
                }
            }
        }
    }

    TopItem {
        id: topItem
    }
}


