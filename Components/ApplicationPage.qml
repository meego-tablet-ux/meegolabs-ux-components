/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Components 0.1
import MeeGo.Labs.Components 0.1 as Labs
import Qt.labs.gestures 2.0

Item {
    id: container
    width: parent.width
    height: parent.height

    property variant windowModel: []
    property variant windowFilterPayload: []

    property string title
    property alias content: contentArea

    property bool fullContent: false
    property bool fullscreen: false
    property bool showSearch: false
    property bool disableSearch: false

    property bool showToolbar: true
    property bool hasBack: false

    property int toolbarHeight: titleBar.height + (showSearch ? searchTitleBar.height : 0)

    property bool moving: false

    property variant applicationData

    /*This is the type of the application
     * 0 - On
     * 1 - Parent off (animation goes from onscreen to offscreen left
     * 2 - Nested off (animation goes from onscreen to offscreen right
     */
    property int type: 2

    property alias menuContent: applicationFilterMenu.content
    property alias menuItem: applicationFilterMenu.contentItem
    property alias menuWidth: applicationFilterMenu.contentWidth
    property alias menuHeight: applicationFilterMenu.contentHeight

    function closeMenu() { applicationFilterMenu.visible = false; }

    signal close();
    signal filterTriggered(int index, variant payload)
    signal addApplicationPage(variant component);
    signal search(string needle)

    onTypeChanged: console.log(title + "application's new type: " + type )

    Component.onCompleted: {
        console.log(title + " is now alive!")
    }

    Component.onDestruction: {
        console.log(title + " is now destroyed!")
        if(type != 2) type = 2;
    }

    onShowSearchChanged: {
        if(!showSearch)
            //clear search
            searchBox.text = ""
    }

    Item {
        id: contentArea
        anchors.top: fullContent ? parent.top : barArea.bottom
        width: parent.width
        anchors.bottom: parent.bottom

        states: [
            State {
                name: "ChildOff"
                when: container.type == 2
                PropertyChanges {
                    target: contentArea
                    x: contentArea.width
                }
            },
            State {
                name: "ParentOff"
                when: container.type == 1
                PropertyChanges {
                    target: contentArea
                    x: -contentArea.width
                }
            },
            State {
                name: "On"
                when: container.type == 0
                PropertyChanges {
                    target: contentArea
                    x: 0
                }
                PropertyChanges {
                    target: container
		    //set showSearch to false will cause the string in search bar be cleaned when loading a detail view. Bug[15576]
                    //showSearch: false 
                }
            }
        ]

        transitions: [
            Transition {
                from: "On"
                to: "ParentOff"
                SequentialAnimation{
                    ScriptAction {
                        script: moving = true
                    }
                    PropertyAnimation {
                        properties: "x"
                        duration: 250
                    }
                    ScriptAction {
                        script: {
                            barArea.visible = false
                            moving = false
                        }
                    }
                }
            },
            Transition {
                from: "ParentOff"
                to: "On"
                SequentialAnimation{
                    PropertyAnimation {
                        properties: "x"
                        duration: 250
                    }
		    ScriptAction { 
		    	 script: {
			     barArea.visible = true
			     container.showSearch = false
			 }	 
		    }
                    
                }
            },
            Transition {
                from: "On"
                to: "ChildOff"
                SequentialAnimation {
                    ScriptAction { script: if (fullContent) barArea.y = - searchTitleBar.height }
                    PropertyAnimation {
                        properties: "x"
                        duration: 250
                    }
                    ScriptAction { script: container.destroy(); }
                }
            },
            Transition {
                from: "ChildOff"
                to: "On"
                PropertyAnimation {
                    properties: "x"
                    duration: 250
                }
		ScriptAction { script: container.showSearch = false }
            }
        ]
    }

    Image {
        id: titleBarShadow
        source: "image://theme/toolbar-shadow"
        anchors.top: barArea.bottom
        width: barArea.width
    }

    Item {
        id: barArea
        width: parent.width
        height: titleBar.height + searchTitleBar.height
        y: (fullContent ? scene.statusBar.y + scene.statusBar.height : 0) - searchTitleBar.height

        BorderImage {
            id: searchTitleBar
            height: 50
            width: parent.width
            source: "image://theme/titlebar_l"

            TextEntry {
                id: searchBox
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                anchors.left: parent.left
                anchors.leftMargin: 10
                onTextChanged: container.search(text)
                defaultText: qsTr("Search")
            }

            states: [
                State {
                    name: "hide"
                    when: !showSearch
                    PropertyChanges {
                        target: barArea
                        y: (fullContent ? scene.statusBar.y + scene.statusBar.height : 0) - searchTitleBar.height
                    }
                },
                State {
                    name: "show"
                    when: showSearch
                    PropertyChanges {
                        target: barArea
                        y: fullContent ? scene.statusBar.y + scene.statusBar.height : 0
                    }
                }
            ]

            transitions: [
                Transition {
                    from: "hide"
                    to: "show"
                    reversible: true
                    PropertyAnimation {
                        properties: "y"
                        duration: 250
                    }
                }
            ]
        }

        BorderImage {
            id: titleBar
            anchors.top: searchTitleBar.bottom
            width: parent.width
            height: backButton.height
            source: "image://theme/titlebar_l"

            // block gestures from falling through
            GestureArea {
                anchors.fill: parent

                Tap {}
                TapAndHold {}
                Pan {}           
                Swipe {}         
                Pinch {}         
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                property int firstY: 0
                property int firstX: 0

                onPressed: {
                    firstY = mouseY;
                    firstX = mouseX;
                }
                onMousePositionChanged: {
                    if(mouseArea.pressed) {
                        if(Math.abs(mouseX - firstX) < Math.abs(mouseY - firstY)) {
                            if( mouseArea.mouseY - mouseArea.firstY > 20) {
                                if(!container.disableSearch)
                                    container.showSearch = true
                            }
                            else if( mouseArea.mouseY - mouseArea.firstY < -20) {
                                container.showSearch = false
                            }
                        }
                    }
                }
            }
            Row {
                anchors.left: parent.left
                height: backButton.height

                GConfItem {
                    id: hasHomeButton
                    defaultValue: false
                    key: "/apps/meego/appsHaveHomeButton"
                }

                Image {
                    id: homeButton
                    visible: hasHomeButton.value
                    fillMode: Image.PreserveAspectFit
                    source: "image://theme/icn_home_dn"
                    height: backButton.height

                    MouseArea {
                        anchors.fill:  parent

                        onClicked: {
                            mainWindow.goHome();
                        }
                    }
                }

                Image {
                    id: backButton
                    source: "image://theme/icn_toolbar_back_button_up"
                    visible: container.hasBack
                    MouseArea {
                        id: backButtonMouseArea
                        anchors.fill: parent

                        onClicked: {
                            container.close();
                        }
                    }

                    states: [
                        State {
                            name: "pressed"
                            when: backButtonMouseArea.pressed
                            PropertyChanges {
                                target: backButton
                                source: "image://theme/icn_toolbar_back_button_dn"
                            }
                        },
                        State {
                            name: "normal"
                            when: !backButtonMouseArea.pressed
                            PropertyChanges {
                                target: backButton
                                source: "image://theme/icn_toolbar_back_button_up"
                            }
                        }

                    ]
                }

                Image {
                    id: spacer
                    visible: backButton.visible
                    source: "image://theme/icn_toolbar_button_divider"
                }
            }





            Text {
                id: toolbarTitleLabel
                anchors.horizontalCenter: parent.horizontalCenter
                text: container.title
                color: theme_toolbarFontColor
                width: parent.width / 2
                elide: Text.ElideRight
                font.pixelSize: theme_toolbarFontPixelSize
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Row {
                anchors.right: parent.right
                height: titleBar.height

                Image {
                    property bool fullContent: false
                    visible: applicationMenuButton.visible
                    source: "image://theme/icn_toolbar_button_divider"
                }

                Image {
                    id: applicationMenuButton
                    visible: windowModel.length > 0
                    source: "image://theme/icn_toolbar_view_menu_up"

                    MouseArea {
                        id: applicationMenuButtonMouseArea
                        anchors.fill: parent
                        onClicked: {
                            var pos = mapToItem(container, applicationMenuButton.x, applicationMenuButton.y)
                            filterMenuComponent.mouseX = pos.x + applicationMenuButton.width / 2
                            filterMenuComponent.mouseY = pos.y + applicationMenuButton.height
                            filterMenuComponent.visible = true;
                        }
                    }

                    Labs.ContextMenu {
                        id: filterMenuComponent
                        model: container.windowModel
                        payload: container.windowFilterPayload
                        fingerMode: 2
                        forceFingerMode: true
                        onTriggered: {
                            container.filterTriggered(index, payload)
                        }
                    }

                    states: [
                        State {
                            name: "pressed"
                            when: applicationMenuButtonMouseArea.pressed
                            PropertyChanges {
                                target: applicationMenuButton
                                source: "image://theme/icn_toolbar_view_menu_dn"
                            }
                        },
                        State {
                            name: "normal"
                            when: !applicationMenuButtonMouseArea.pressed
                            PropertyChanges {
                                target: applicationMenuButton
                                source: "image://theme/icn_toolbar_view_menu_up"
                            }
                        }

                    ]
                }

                Image {
                    id: spacer2
                    visible: applicationMenuButton.visible
                    source: "image://theme/icn_toolbar_button_divider"
                }

                Rectangle {
                    anchors.fill: windowMenuButton
                    visible: !windowMenuButton.visible
                    color: "transparent"
                }

                Image {
                    id: windowMenuButton
                    visible: false
                    source: "image://theme/icn_toolbar_applicationpage_menu_up"

                    MouseArea {
                        id: windowMenuButtonMouseArea
                        anchors.fill: parent
                        onClicked: {
                            var pos = mapToItem(container, 0, 0)
                            applicationFilterMenu.mouseX = pos.x + windowMenuButton.width / 2
                            applicationFilterMenu.mouseY = pos.y + windowMenuButton.height
                            applicationFilterMenu.visible = true;
                        }
                    }

                    AbstractContext {
                        id: applicationFilterMenu
                        content: menuContent
                        fingerMode: 2
                        forceFingerMode: true

                        Component.onCompleted: {
                            windowMenuButton.visible = applicationFilterMenu.content != null
                        }

                        onContentChanged: {
                            windowMenuButton.visible = true;
                        }
                    }

                    states: [
                        State {
                            name: "pressed"
                            when: windowMenuButtonMouseArea.pressed
                            PropertyChanges {
                                target: windowMenuButton
                                source: "image://theme/icn_toolbar_applicationpage_menu_dn"
                            }
                        },
                        State {
                            name: "normal"
                            when: !windowMenuButtonMouseArea.pressed
                            PropertyChanges {
                                target: windowMenuButton
                                source: "image://theme/icn_toolbar_applicationpage_menu_up"
                            }
                        }
                    ]
                }
            }
        }
    }
}
