/*
* Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import QtMobility.contacts 1.1
import MeeGo.Labs.Components 0.1 as Labs
import MeeGo.Components 0.1

/*
* Contacts Picker
 *
 * signals:
 * contactSelected(variant contact)
 * - qdeclarativecontact of selected contact
 *
 * dataSelected(string type, int dataIndex) //not implemented yet
 * - type {"Phone", "Email", "IM"} and index of data,
 * - usage: if(type == "Phone") contact.phones[dataIndex];
 *
 * parameters:
 * promptString - Title of Picker Dialog
 * filterFlags - int, union of data types to filter by {Name=0, Phone, Email, IM}
 * 
 * functions: 
 *  show() - shows the contact picker with the current list of contacts displayed
 *
 */

ModalDialog {
    id: contactPicker

    property string promptString: qsTr("Pick a contact:")
    property int filterFlags: 0 //not implemented yet

    property string dataType: "" //not implemented yet
    property int dataIndex: 0 //not implemented yet
    property variant person: null

    title: promptString

    signal closed
    signal opened
    signal contactSelected(variant contact)
    signal dataSelected(string type, int dataIndex)
    signal cancelled

    width: 300
    height: 450

    onAccepted:{
        contactPicker.contactSelected(contactPicker.person)
        contactPicker.dataSelected(contactPicker.dataType, contactPicker.dataIndex)
        contactPicker.closed();
    }

    onRejected:{
        contactPicker.cancelled();
    }

    Theme{ id:theme }

    content: Item {
            id:contactsView

            property int highlightHeight: 0
            property int highlightWidth: 0
            property int highlightX: 0
            property alias contactListView: cardListView

            Component {
                id: cardHighlight
                Rectangle {
                    opacity: .5
                    color: "lightgrey"
                    y: cardListView.currentItem.y
                    width: contactsView.highlightWidth; height: contactsView.highlightHeight
                    x: contactsView.highlightX
                    z: 2
                    Behavior on y { SmoothedAnimation { velocity: 5000 } }
                }
            }

            anchors.fill: parent
            clip:  true

            Column {
                id:pickerContents
                spacing: 5

   		height: parent.height
                width: parent.width

		Item {
                    id:loadingText
                    height:30
                    width:contactsView.width
                    Text {
                        id: promptText
                        text: qsTr("Loading...")
                        color:theme.fontColorHighlight
                        font.pixelSize: theme.fontPixelSizeLarge
                        anchors.horizontalCenter: parent.horixontalCenter
                        visible: true
			opacity: 1
                    } // prompt text
                }//loadingText

                Item {
                    id: groupedViewPortrait
                    width: parent.width
//		    height: parent.height-titleBox.height-buttonBox.height-10-loadingText.height
                    height: parent.height-10-loadingText.height

                    ListView {
                        id: cardListView
                        height: parent.height
                        width: groupedViewPortrait.width
                        snapMode: ListView.SnapToItem
                        highlightFollowsCurrentItem: false
                        focus: true
                        keyNavigationWraps: false
                        clip: true
                        opacity: 1

                        Component.onCompleted: {
                            positionViewAtIndex(-1, ListView.Beginning);
                            cardListView.currentIndex = -1 // force to -1 since currentIndex inits with 0.
                        }

                        highlight: cardHighlight
                        model: ContactModel {
                            id: contactModel
                            autoUpdate: true
                            Component.onCompleted : {
                                contactModel.sortOrders= sortFirstName;
                                contactModel.filter= allFilter;
                                if(manager == "tracker")
                                    console.debug("[contacts:myappallcontacts] tracker found for all contacts model")
                                }
                                }//model
                                    SortOrder {
                                        id: sortFirstName
                                        detail:ContactDetail.Name
                                        field:Name.FirstName
                                        direction:Qt.AscendingOrder
                                    }
                                    IntersectionFilter {
                                        id: allFilter
                                    }

                                    delegate:  Image {
                                        id: contactCardPortrait

                                        height: 50
                                        width: parent.width
                                        opacity: 1

                                        property variant dataContact: model.contact
                                        property string dataUuid: model.contact.guid.guid
                                        property string dataFirst: model.contact.name.firstName
                                        property string dataLast: model.contact.name.lastName
                                        property string dataCompany: model.contact.organization.name
                                        property string dataFavorite: model.contact.tag.tag
                                        property int dataStatus: model.contact.presence.state
                                        property string dataAvatar: model.contact.avatar.imageUrl

                                        signal clicked

                                        source: "image://theme/contacts/contact_bg_portrait";

                                        Image{
                                            id: photo
                                            fillMode: Image.PreserveAspectFit
                                            smooth: true
                                            width: 50
                                            height: 50
                                            source: (dataAvatar ? dataAvatar :"image://theme/contacts/blank_avatar")
                                            anchors {left: contactCardPortrait.left;}
                                        }

                                        Text{
                                            id: nameFirst
                                            height: 20
                                            text: dataFirst
                                            anchors { left: photo.right; top: photo.top; topMargin: 2; leftMargin: 2;}
                                            font.pixelSize: theme.fontPixelSizeNormal
                                            color: theme.fontColorNormal
                                        }

                                        Text {
                                            id: nameLast
                                            text: dataLast
                                            anchors { left: nameFirst.right; top: nameFirst.top; leftMargin: 2;}
                                            font.pixelSize: theme.fontPixelSizeNormal
                                            color: theme.fontColorNormal
                                        }

                                        Image {
                                            id: favorite
                                            source: "image://theme/contacts/icn_fav_star"
                                            height: 10
                                            width: 10
                                            opacity: (dataFavorite == "Favorite" ? 1 : .2 )
                                            anchors {right: contactCardPortrait.right; top: nameFirst.top; rightMargin: 2;}
                                        }

                                        Image {
                                            id: statusIcon
                                            height: 10
                                            width: 10
                                            source: {
                                                if(dataStatus == Presence.Unknown)
                                                    return "image://theme/contacts/status_idle";
                                                else if (dataStatus == Presence.Available)
                                                    return "image://theme/contacts/status_available";
                                                else if (dataStatus == Presence.Busy)
                                                    return "image://theme/contacts/status_busy_sml";
                                                else
                                                    return "image://theme/contacts/status_idle";

                                            }
                                            anchors {horizontalCenter: favorite.horizontalCenter; bottom: photo.bottom; bottomMargin:2; rightMargin: 2; }
                                        }

                                        Text {
                                            id: statusText
                                            text: {
                                                if (dataStatus == Presence.Unknown)
                                                    return qsTr("Idle");
                                                else if (dataStatus == Presence.Available)
                                                    return qsTr("Available");
                                                else if (dataStatus == Presence.Busy)
                                                    return qsTr("Busy");
                                                else
                                                    return ""
                                            }
                                            anchors { left: nameFirst.left; bottom: photo.bottom; bottomMargin: 2}
                                            font.pixelSize: theme.fontPixelSizeSmall
                                            color: theme.fontColorHighlight
                                        }

                                        Image{
                                            id: contactDivider
                                            source: "image://theme/contacts/contact_divider"
                                            anchors {right: contactCardPortrait.right; bottom: contactCardPortrait.bottom; left: contactCardPortrait.left; }
                                        }

                                        MouseArea {
                                            id: mouseArea
                                            anchors.fill: contactCardPortrait
                                            onClicked: {
                                                contactCardPortrait.clicked()
                                                console.log("contact clicked"+index)
                                                cardListView.currentIndex = index
                                                cardListView.currentItem.state = "selected"
                                                contactPicker.person = dataContact
                                                contactsView.highlightX = cardListView.currentItem.height // Assume image fills Card height and is square
                                                contactsView.highlightWidth = parent.width - contactsView.highlightX
                                                contactsView.highlightHeight = contactsView.highlightX
                                            }
                                        }

                                        states: [
                                            State {
                                                name: "selected"; when: contactListView.currentIndex == index;
                                                PropertyChanges { target: contactCardPortrait; opacity: .7;}
                                            },
                                            State{
                                                name: "unselected"; when: contactListView.currentIndex != index;
                                                PropertyChanges { target: contactCardPortrait; opacity: 1; }
                                            }
                                        ]
                                    }//contactCardPortrait

                                    section.property: "dataFirst"
                                    section.criteria: ViewSection.FirstCharacter
                                    section.delegate: Image {
                                        id: header

                                        width: parent.width
                                        height: 30

                                        source: "image://theme/contacts/contact_btmbar_landscape";
                                        clip: true

                                        Text {
                                            id: title
                                            text: section
                                            anchors { fill: parent; rightMargin: 6; leftMargin: 20; topMargin: 6; bottomMargin: 6; verticalCenter: header.verticalCenter }
                                            font.pixelSize: theme.fontPixelSixeNormal
                                            color: theme.fontColorHighlight
                                        }
                                    }

                                }

                    Binding{target: loadingText; property: "opacity"; value: 0; when: cardListView.count > 1}
                    Binding{target: loadingText; property: "visible"; value: 1; when: cardListView.count > 1}

                }//portraitGroupedView
            }//Column
        }
}//ContactPicker

