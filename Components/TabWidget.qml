/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Item {
    id: tabWidget
    height: tabContainer.height + 30
    width: contentWidth + tabIcons.width

    property color containerColor: "gray"
    property int contentWidth: 300
    property variant model
    property int currentTabIndex: -1
    property variant nextModel:undefined
    property int viewMode: 0
    property variant tabs: tabIcons.children
    property variant currentTab
    currentTab: {
        if (currentTabIndex == -1) {
            undefined;
        }else {
            tabContainer.children[tabWidget.currentTabIndex].item;
        }
    }

    clip: true

    // change the content
    onCurrentTabIndexChanged: {
        if (currentTabIndex > -1 && currentTabIndex < model.length) {
            if(tabContainer.children[currentTabIndex].item != undefined) {
                tabContainer.children[currentTabIndex].item.opacity = 1;
            }else {
                tabContainer.children[currentTabIndex].sourceComponent = model[currentTabIndex].contentComponent;
            }
        }
    }
    function setModel(components) {
        viewMode = 2;
        nextModel = components;
    }
    function clear() {
        viewMode = 2;
    }

    function open() {
        viewMode = 1;
    }
    function close() {
        viewMode = 0;
    }

    signal tabTriggered(int index)

    onTabTriggered : {
        var t = currentTabIndex;
        selectTabAtIndex(index);

        if (viewMode == 1 && t == index)
        {
            close();
        }
        else
        {
            open();
        }
    }


    function selectTabAtIndex(index) {
        if (index >= tabicons.count || index < 0) {
            return;
        }
        else
        {
            if (currentTabIndex != index)
            {
                if (currentTabIndex != -1)
                {
                    tabIcons.children[currentTabIndex].item.deselect();
                    tabContainer.children[currentTabIndex].item.opacity = 0;
                }
                currentTabIndex = index;
                tabIcons.children[index].item.select();
                //    tabWidget.viewMode = 1;
            }
            else
            {
                //   tabWidget.viewMode = 1 - tabWidget.viewMode;
            }
        }
    }

    function deselect() {
        if (currentTabIndex > -1 && currentTabIndex < tabicons.count)
        {
            tabIcons.children[currentTabIndex].item.deselect();
            currentTabIndex = -1;
        }
    }

    BorderImage {
        id: tabBackgroundTop
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: -10
        width: parent.width - tabIcons.width + 20
        source: "image://theme/tabmenu_bg_3"
        border.left: 20
        border.top: height
        border.right: 10
        border.bottom: 0
    }
    BorderImage {
        id: tabBackgroundMiddle
        anchors.top: tabBackgroundTop.bottom
        anchors.left: parent.left
        anchors.leftMargin: -10
        source: "image://theme/tabmenu_bg_2"
        width: parent.width - tabIcons.width + 20
        height: parent.height - tabBackgroundTop.height - tabBackgroundBottom.height
        border.left: 20; border.top: 0
        border.right: 20; border.bottom: 0
    }
    BorderImage {
        id: tabBackgroundBottom
        anchors.top: tabBackgroundMiddle.bottom
        anchors.left: parent.left
        anchors.leftMargin: -10
        source: "image://theme/tabmenu_bg_1"
        width: parent.width - tabIcons.width + 20
        border.left: 20; border.top: 0
        border.right: 20; border.bottom: 0
    }

    Item {
        id: tabContainer
        width: tabWidget.contentWidth
        height: {
            if (children[currentTabIndex] == undefined || children[currentTabIndex].item == undefined)
            {
                return tabIcons.height
            }
            else
            {
                return Math.max(tabIcons.height, children[currentTabIndex].item.height);
            }
        }
        anchors.top:parent.top
        anchors.left:parent.left
        z: 10
        Repeater {
            id: tabs
            model: tabWidget.model
            Loader {
                id: tabLoader
            }
        }
    }
    MouseArea{
        // avoid clicking on the maincontent behind the side content
        anchors.left:tabContainer.left
        anchors.top:tabContainer.top
        width:tabContainer.width
        height:tabContainer.height
        z: 0
    }

    Column {
        id: tabIcons
        width: childrenRect.width
        height:childrenRect.height
        anchors.left: tabContainer.right
        anchors.top:parent.top
        Repeater {
            id: tabicons
            model: tabWidget.model
            Loader {
                id: iconLoader
                sourceComponent: modelData.iconComponent
                onStatusChanged: {
                    if (status == Loader.Ready) {
                        item.payload = index;
                    }
                }
                Connections {
                    target:iconLoader.item
                    onTriggered: {
                        tabWidget.tabTriggered(index);
                    }
                }
            }

        }
    }
    state:"partialhidden"
    states: [
        State {
            name: "partialhidden"
            when: viewMode == 0
            PropertyChanges {
                target: tabWidget
                x: -contentWidth
            }
        },
        State {
            name: "shown"
            when: viewMode == 1
            PropertyChanges {
                target: tabWidget
                x: 0
            }
        },
        State {
            name: "fullyhidden"
            when: viewMode == 2
            PropertyChanges {
                target: tabWidget
                x: -width
            }
        }
    ]

    transitions: [
        Transition {
            reversible: true
            SequentialAnimation {
                PropertyAnimation {
                    property: "x"
                    duration:300
                }
                ScriptAction {
                    script: {
                        if (viewMode == 2) {
                            deselect();
                            if (nextModel == undefined) {
                                model = undefined;
                            }else {
                                model = nextModel;
                                nextModel = undefined;
                            }
                            viewMode = 0;
                        }
                    }
                }
            }
        }
    ]
}
