/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import QtMobility.sensors 1.1
import "helper.js" as WindowHelper

Item {
    id: scene

    // If we are running using the executable wrapper, then we will have a
    // screenWidth and screenHeight added to our context.  If running via
    // qmlviewer then fallback to a reasonable default
    width: {
        try
        {
            screenWidth;
        }
        catch (err)
        {
            1280;
        }
    }
    height: {
        try
        {
            screenHeight;
        }
        catch (err)
        {
            800;
        }
    }

    property bool foreground: qApp.foregroundWindow == mainWindow.winId
    onForegroundChanged: {
        if (foreground && !orientationLocked)
        {
            orientationTransitionEnabled = false;
            orientation = qApp.orientation
            transitionDisableTimer.running = true;
            qApp.orientationLocked = false;
        }
        else if (!foreground)
        {
            qApp.orientationLocked = true;
        }
    }

    Component.onCompleted: {
        try {
            mainWindow.actualOrientation = orientation;
            qApp.appPageLoaded();
        }
        catch (err) { }
    }

    property bool transparent: false
    property string title
    property variant filterModel: []
    property variant filterPayload: []

    ///Deprecated:
    property int filterMenuWidth: 0

    signal filterTriggered(int index, variant payload)

    property Component applicationPage: null
    property alias currentApplication: privateData.application
    property variant applicationData

    onApplicationDataChanged: {
        privateData.application.applicationData = applicationData
    }

    onFilterModelChanged: {
        if(!privateData.application) return;
        privateData.application.windowModel = scene.filterModel
        var i=0
        for(i;i<WindowHelper.applications.length;i++) {

            WindowHelper.applications[i].windowModel = scene.filterModel
            WindowHelper.applications[i].windowFilterPayload = scene.filterPayload
        }
    }

    onApplicationPageChanged: {
        if(!applicationPage) return;

        if(!privateData.suppress) {

            if(privateData.application){
                privateData.application.destroy();
                privateData.application = null
            }

            for(var i=0;i<WindowHelper.applications.length;i++){
                WindowHelper.applications[i].destroy();
            }

            WindowHelper.applications = []

        }



        if(applicationPage.status == Component.Ready) {
            finalizeAppPageChange();
        }
        else {
            applicationPage.statusChanged.connect(finalizeAppPageChange);
        }

    }

    function finalizeAppPageChange() {
        if(applicationPage.status == Component.Ready) {
            privateData.application = applicationPage.createObject(content);
            privateData.application.windowModel = scene.filterModel
            privateData.application.windowFilterPayload = scene.filterPayload
            privateData.application.type = privateData.pageCreatedType
            privateData.pageCreatedType = 0; ///reset this back to default
            privateData.application.addApplicationPage.connect(addApplicationPage)
            privateData.application.close.connect(previousApplicationPage)
            privateData.application.filterTriggered.connect(scene.filterTriggered)

            privateData.application.hasBack = WindowHelper.applications.length > 0
            privateData.application.showToolbar = scene.showtoolbar
            privateData.application.applicationData = scene.applicationData
            privateData.fullContent = privateData.application.fullContent
            privateData.fullscreen = privateData.application.fullscreen

            if(!privateData.suppress) {
                scene.title = privateData.application.title
                console.log("setting view title to be: " + scene.title)
            }

            privateData.application.title = scene.title
        }
        else console.log("app is not ready!" + applicationPage.status)

        //reset suppression:
        privateData.suppress = false;
    }

    function addApplicationPage(component) {
        //Hide current page:

        var stackLength = WindowHelper.applications.length
        if (stackLength > 0) {
            var lastPage = WindowHelper.applications[stackLength - 1]
            if (lastPage.moving) {
                console.log("Warning: attempted to add app page while still animating - aborting!")
                return
            }
        }

        privateData.application.type = WindowHelper.TypeParentPageOff //current page goes offscreen left
        WindowHelper.applications.push(privateData.application)


        privateData.suppress = true;
        privateData.pageCreatedType = WindowHelper.On

        applicationPage = component;
    }

    function previousApplicationPage() {
        if(WindowHelper.applications.length == 0) return;

        privateData.application.type = WindowHelper.TypeChildPageOff //current page goes offscreen right
        applicationPage = null;

        if(WindowHelper.applications.length > 0) {
            privateData.application = WindowHelper.applications.pop();
            privateData.application.type = WindowHelper.On
            privateData.fullContent = privateData.application.fullContent
            privateData.fullscreen = privateData.application.fullscreen

        }
        else console.log("there doesn't seem to be any more pages")
    }

    Connections {
        target: privateData.application
        onFullContentChanged: {
            privateData.fullContent = privateData.application.fullContent
        }
        onFullscreenChanged: {
            privateData.fullscreen = privateData.application.fullscreen
        }
    }

    property alias content: innerContent
    property alias container: outerContent

    // We support four orientations:
    // 0 - Right Uup
    // 1 - Top Up
    // 2 - Left Up
    // 3 - Top Down
    property int orientation: qApp.orientation
    onOrientationChanged: {
        mainWindow.actualOrientation = orientation;
    }

    property bool orientationLocked: false
    onOrientationLockedChanged: {
        qApp.orientationLocked = orientationLocked;
    }

    // Disable orientation transitions on startup
    property bool orientationTransitionEnabled: false
    Timer {
        id: transitionDisableTimer
        running: true
        repeat: false
        interval: 1000
        onTriggered: scene.orientationTransitionEnabled = true;
    }

    property alias fullContent: privateData.fullContent
    property alias fullscreen: privateData.fullscreen
    property bool showtoolbar: true
    property bool showsearch: false ///this is deprecated!!

    onShowtoolbarChanged: {
        if(privateData.application)
            privateData.application.showToolbar = showtoolbar
    }

    signal search(string needle)

//    property alias statusBar: statusbar

    property color backgroundColor: theme_backgroundColor
    property color sidepanelBackgroundColor: theme_sidepanelBackgroundColor

    signal statusBarTriggered()

    property bool showStartupAnimation: false

    Connections {
        target: qApp
        onOrientationChanged: {
            if (scene.foreground)
            {
                scene.orientation = qApp.orientation;
            }
        }
    }

    // private
    Item {
        id: privateData

        property Item contextMenuItem: null
        property ApplicationPage application: null
        property int pageCreatedType: 0
        property bool fullContent: false
        property bool fullscreen: false
        property bool suppress: false
        Translator {
            catalog: "meego-ux-components"
        }

    }

    // This function returns if the view is in landscape or inverted landscape
    function isLandscapeView() {
        return orientation == 1 || orientation == 3;
    }

    // Clamp val between min and max
    function clamp (val, min, max) {
        return Math.max (min, Math.min (max, val));
    }

    function openContextMenu(component, loader, mouseX, mouseY, payload,model)
    {
        loader.sourceComponent = component
        loader.item.parent = outerContent
        loader.item.model = model
        // The menu container consist of an Item the size of the
        // Window outerContent that renders a fog effect by use of
        // a child rectangle with an opacity, and a child element
        // that represents the actual menu that must be placed at
        // the correct location.
        //
        // Clicking in the fog should close the context menu and
        // clicking on the actual menu should result in a triggered
        // signal being sent containing the selected index
        var menuContainer = loader.item
        menuContainer.width = outerContent.width
        menuContainer.height = outerContent.height
        menuContainer.z = 100
        menuContainer.payload = payload

        // Ensure that the context menu will completely fit
        // on the screen, and that the menu finger is attatched
        // to the correct corner so its obvious what element
        // the context menu is associated with.
        //
        // menuContainer.fingerMode:
        // 0 - left
        // 1 - right
        // 2 - top
        // 3 - bottom

        // These offsets are the distance from the point of the finger
        // (at the mouse click) to the appropriate edge of the menu
        // images. They are different due to the amounts of dropshadow
        // and translucency around the different edges.
        var leftOffset = 1;
        var rightOffset = 1;
        var topOffset = 14;
        var bottomOffset = 15;

        mouseY -= scene.statusBar.height;

        // Step one, find the appropriate direction for the finger to point
        // Horizontal placement takes precedence over vertical.
        if (mouseX + leftOffset + menuContainer.menuWidth < outerContent.width) {
            menuContainer.fingerMode = 0;

            // Check vertically
            if (mouseY + (menuContainer.menuHeight / 2) > outerContent.height) {
                // Switch to bottom

                menuContainer.fingerMode = 3;
            } else if (mouseY - (menuContainer.menuHeight / 2) < 0) {
                // Switch to top
                menuContainer.fingerMode = 2;
            }
        } else if (mouseX - menuContainer.menuWidth - rightOffset >= 0) {
            menuContainer.fingerMode = 1;

            if (mouseY + (menuContainer.menuHeight / 2) > outerContent.height) {
                menuContainer.fingerMode = 3;
            } else if (mouseY - (menuContainer.menuHeight / 2) < 0) {
                menuContainer.fingerMode = 2;
            }
        }

        // Step two, given the finger direction, find the correct location
        // for the menu, keeping it onscreen.
        switch (menuContainer.fingerMode) {
        case 0:
        case 1:
            menuContainer.menuY = clamp (mouseY - menuContainer.menuHeight / 2, 0, menuContainer.height);

            if (menuContainer.fingerMode == 0) {
                menuContainer.menuX = mouseX + leftOffset;
            } else {
                menuContainer.menuX = mouseX - rightOffset - menuContainer.menuWidth;
            }
            break;

        case 2:
        case 3:
            // Clamp mouseX so that at the edges of the screen we don't
            // try putting the finger at a location where the menu can't be
            // placed
            // We don't need to do something similar to mouseY because when
            // it would happen to mouseY, we have switched to top or bottom
            // fingermode, and so it because a mouseX issue
            mouseX = clamp (mouseX, 35, outerContent.width - 35);

            menuContainer.menuX = clamp (mouseX - menuContainer.menuWidth / 2, 0, (menuContainer.width - menuContainer.menuWidth));
            if (menuContainer.fingerMode == 2) {
                menuContainer.menuY = mouseY + topOffset;
            } else {
                menuContainer.menuY = mouseY - menuContainer.menuHeight + bottomOffset;
            }
            break;
        }

        menuContainer.fingerX = mouseX;
        menuContainer.fingerY = mouseY;

        // Fade the menu into the scene
        menuContainer.visible = true;
    }

    function showModalDialog (source) {
        if (typeof (source) == "string") {
            dialogLoader.source = source;
        } else {
            dialogLoader.sourceComponent = source;
        }
        dialogLoader.item.parent = scene.container;
    }

    Item {

        id: outerContent

        anchors.centerIn: parent

        width: (rotation == 90 || rotation == -90) ? parent.height : parent.width
        height: (rotation == 90 || rotation == -90) ? parent.width : parent.height
        clip:true

        property string oldState : ""

        Image {
            id: background
            opacity: scene.transparent ? 0.0 : 1.0
            anchors.fill: parent
            fillMode: Image.Tile
            source: "image://theme/bg_application_p"
        }

        StatusBar {
            id: statusBar

            x: 0
            y: if( fullContent ){
                - statusBar.height - clipBox.height
            }
            else if( privateData.fullscreen ){
                - statusBar.height
            }
            else{
                0
            }
            width: outerContent.width
            height: 30
            z: 1

            Behavior on y {
                PropertyAnimation {
                    duration: theme.dialogAnimationDuration
                    easing.type: "OutSine"
                }
            }
        } //end of statusBar

        Item {
            id: innerContent
            anchors.top: statusBar.bottom
            anchors.left: outerContent.left
            anchors.bottom: parent.bottom
            width: outerContent.width

        }

        states:  [
            State {
                name: "windowHasNoFocus"
                when: (!isActiveWindow)
            },
            State {
                name: "landscape"
                when: scene.orientation == 1
                PropertyChanges {
                    target: outerContent
                    rotation: 0
                    width: parent.width
                    height: parent.height
                }
            },
            State {
                name: "invertedLandscape"
                when: scene.orientation == 3
                PropertyChanges {
                    target: outerContent
                    rotation: 180
                    width: parent.width
                    height: parent.height
                }
            },
            State {
                name: "portrait"
                when: scene.orientation == 2
                PropertyChanges {
                    target: outerContent
                    rotation: -90
                    width: parent.height
                    height: parent.width
                }
            },
            State {
                name: "invertedPortrait"
                when: scene.orientation == 0
                PropertyChanges {
                    target: outerContent
                    rotation: 90
                    width: parent.height
                    height: parent.width
                }
            }
        ]
        transitions: Transition {
            from: scene.orientationTransitionEnabled ? "*" : "nosuchstate"
            to: "*"
            SequentialAnimation {
                ParallelAnimation {
                    PropertyAction {
                        target: statusBar;
                        property: "width";
                        value: outerContent.width
                    }
                    RotationAnimation {
                        target: outerContent
                        direction: RotationAnimation.Shortest;
                        duration: isActiveWindow ? 220 : 0
                    }
                    PropertyAnimation {
                        target: outerContent
                        properties: "width,height"
                        duration: isActiveWindow ? 220 : 0
                        easing.type: "OutSine"
                    }
                }
            }
        }
    }
 }
