/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

/* This pages opens the TimezoneListView.qml and offers controls to manipulate it.
 */

import Qt 4.7
import MeeGo.Components 0.1
import MeeGo.Labs.Components 0.1 as Labs

WidgetPage {
    id: widgetPage

    property string lastSignal: "none"
    property string contactName: "-"

    pageTitle: qsTr("TimezoneListView Testing")

    controlContent: [
/*
        LabeledEntry {
            id: widthEntry

            label:  qsTr("sizeHintWidth: ")
            defaultValue: timezoneLV.sizeHintWidth

            onTextUpdated: {
                if( value >= 0 ){
                    timezoneLV.sizeHintWidth = value
                }
            }
        }
*/
    ]

    statusContent: [
/*
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
*/
    ]

    description: qsTr("The TimezoneListView provides a listview in which the user can choose a timezone.")

    widget: Flickable {

        anchors.fill: parent
        contentHeight: height
        contentWidth: width 
        clip: true

        Labs.TimezoneListView {
            id: timezoneLV

            anchors.centerIn : parent
            width: parent.width / 2
            height: parent.height
/*
            onContactSelected: {
                lastSignal = "contactSelected"
                contactName = contact.name.firstName + " " + contact.name.lastName
            }

            onRejected: {
                lastSignal = "rejected"
                contactName = "-"
            }
*/
        }
    }
}

