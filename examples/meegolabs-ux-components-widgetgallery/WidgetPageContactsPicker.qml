/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

/* This pages opens the ContactsPicker.qml and offers controls to manipulate it.
 */

import Qt 4.7
import MeeGo.Components 0.1
import MeeGo.Labs.Components 0.1 as Labs

WidgetPage {
    id: widgetPage

    property string lastSignal: "none"
    property string contactName: "-"

    pageTitle: qsTr("ContactsPicker Testing")

    controlContent: [
        LabeledEntry {
            id: widthEntry

            label:  qsTr("sizeHintWidth: ")
            defaultValue: contactsPicker.sizeHintWidth

            onTextUpdated: {
                if( value >= 0 ){
                    contactsPicker.sizeHintWidth = value
                }
            }
        },

        LabeledEntry {
            id: heightEntry

            label:  qsTr("sizeHintHeight: ")
            defaultValue: contactsPicker.sizeHintHeight

            onTextUpdated: {
                if( value >= 0 ){
                    contactsPicker.sizeHintHeight = value
                }
            }
        }
    ]

    statusContent: [
        StatusEntry {
            id: lastSignalBox

            label: qsTr("Last signal sent:")
            value: lastSignal
        },
        StatusEntry {
            id: nameBox

            label: qsTr("Contact name:")
            value: contactName
        }
    ]

    description: qsTr("The ContactsPicker provides a modal dialog in which the user can choose a "
                  + "contact.")


    widget: Button {
        id: button

        anchors.centerIn: parent
        width:  300
        height:  80
        text: qsTr("Show ContactsPicker")

        onClicked: {
            contactsPicker.show()
        }
    }

    Labs.ContactsPicker {
        id: contactsPicker

        onContactSelected: {
            lastSignal = "contactSelected"
            contactName = contact.name.firstName + " " + contact.name.lastName
        }

        onRejected: {
            lastSignal = "rejected"
            contactName = "-"
        }
    }
}

