/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

/* This pages opens the VideoPicker.qml and offers controls to manipulate it.
 */

import Qt 4.7
import MeeGo.Components 0.1
import MeeGo.Labs.Components 0.1 as Labs

WidgetPage {
    id: widgetPage

    property string lastSignal: "none"
    property string pickedTitle: "-"
    property string pickedId: "-"
    property string pickedUri: "-"
    property string pickedThumbUri: "-"
    property string pickedItems: "-"

    pageTitle: qsTr("DocumentPicker Testing")

    controlContent: [
        LabeledEntry {
            id: widthEntry

            label:  "width: "
            defaultValue: documentPicker.width

            onTextUpdated: {
                if( value >= 0 ){
                    documentPicker.width = value
                }
            }
        },

        LabeledEntry {
            id: heightEntry

            label:  "height: "
            defaultValue: documentPicker.height

            onTextUpdated: {
                if( value >= 0 ){
                    documentPicker.height = value
                }
            }
        },

        CheckBoxEntry {
            id: selectMultiBox

            label: "multiSelection:"
            onCheckedChanged: {
                lastSignal = "none"
                pickedId = "-"
                pickedTitle = "-"
                pickedUri = "-"
                pickedThumbUri = "-"
                pickedItems = "-"
            }
        }
    ]

    statusContent: [
        StatusEntry {
            id: lastSignalBox

            label: "Last signal sent:"
            value: lastSignal
        },

        StatusEntry {
            id: idEntry

            label: "Id:"
            value: pickedId
        },

        StatusEntry {
            id: titleEntry

            label: "Title:"
            value: pickedTitle
        },

        StatusEntry {
            id: uriEntry

            label: "Uri:"
            value: pickedUri
        },

        StatusEntry {
            id: thumbUriEntry

            label: "ThumbUri:"
            value: pickedThumbUri
        },

        StatusEntry {
            id: itemsEntry

            label: "Selected items:"
            value: pickedItems
            visible: false
        }
    ]

    description: "The DocumentPicker provides a modal dialog in which the user can choose an "
                  + "document. The 'Ok' button is disabled until a selection was made. "
                  + "On 'Ok'-clicked, depending on the selection mode, the fitting signal is "
                  + "emitted which provides the selected item's data. Multi selection of items "
                  + "is possible by setting multiSelection via the API property.<br>"
                  + "Dialogs are centered by default with a vertical offset to to keep the toolbar visible."

    widget: Button {
        id: button

        anchors.centerIn: parent
        width:  300
        height:  80
        text: "Show DocumentPicker"

        onClicked: {
            documentPicker.show()
        }
    }

    Labs.DocumentPicker {
        id: documentPicker

        multiSelection: selectMultiBox.isChecked

        onDocumentSelected: {
            lastSignal = "documentSelected()"
            pickedId = itemid
            pickedTitle = itemtitle
            pickedUri = uri
            pickedThumbUri = thumbUri
        }

        onRejected: {
            lastSignal = "rejected()"
            pickedId = "no signal sent"
            pickedTitle = "no signal sent"
            pickedUri = "no signal sent"
            pickedThumbUri = "no signal sent"
            pickedItems = "-"
        }

        onMultipleDocumentsSelected: {
            lastSignal = "multipleDocumentsSelected()"
            pickedItems = ids.length
        }
    }

    states: [
        State {
            name: "multiple"
            PropertyChanges {
                target: idEntry
                visible: false
            }
            PropertyChanges {
                target: titleEntry
                visible: false
            }
            PropertyChanges {
                target: uriEntry
                visible: false
            }
            PropertyChanges {
                target: thumbUriEntry
                visible: false
            }
            PropertyChanges {
                target:  itemsEntry
                visible: true
            }

            when: selectMultiBox.isChecked
        }
    ]
}
