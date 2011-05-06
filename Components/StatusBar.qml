import Qt 4.7

Item {
    id: container
    property variant networkIndicator: null
    property variant clockIndicator: null
    property variant batteryIndicator: null
    property bool active: true
    property alias backgroundOpacity: background.opacity
    property bool showClock: true

    Item {
        id: privateData
        property variant notifyBanner: null
    }

    function showBanner(message) {
        if (privateData.notifyBanner) {
            return;
        }

        privateData.notifyBanner = notifyComponent.createObject(container);
        privateData.notifyBanner.text = message;
    }

    Component {
        id: notifyComponent
        Rectangle {
            id: notifyInstance
            x: 0
            y: -height
            Behavior on y {
                PropertyAnimation { duration: 200; }
            }
            onYChanged: {
                if (y == -height)
                    notifyInstance.destroy();
            }

            width: container.width
            height: container.height
            color: theme_statusBarBackgroundColor

            Component.onCompleted: y = 0

            property alias text: notifyText.text
            Text {
                id: notifyText
                anchors.fill: parent
                anchors.leftMargin: 10
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: theme_statusBarFontPixelSize
                elide: Text.ElideRight
                color: theme_statusBarFontColor
            }
            Timer {
                id: notifyTimer
                running: true
                interval: 3000
                onTriggered: notifyInstance.y = -notifyInstance.height
            }
        }
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: theme_statusBarBackgroundColor
        opacity: theme_statusBarOpacity
    }
    Row {
        anchors.left: parent.left
        NetworkIndicator {
            active: container.active
        }
        BluetoothIndicator {
            hideOnActiveNetwork: true
            active: container.active
        }
    }
    Text {
        anchors.centerIn: parent
        height: parent.height
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: theme_statusBarFontPixelSize
        color: theme_statusBarFontColor
        opacity: container.showClock ? 1.0 : 0.0
        LocalTime {
            id: localTime
            interval: active ? 60000 : 0
        }
        text: localTime.shortTime
    }
    Image {
        anchors.centerIn: parent
        opacity: container.showClock ? 0.0 : 1.0
        source:  "image://meegotheme/icons/statusbar/locked"
    }

    NotificationIndicator {
        id: notificationIndicator
        anchors.right: volumeIndicator.left
        active: container.active
        onNotify: {
            var msg = qsTr("%1 : %2").arg(summary).arg(body) //summary + ": " + body;
            showBanner(msg.replace(/\n/g, ' '));
        }
    }
    Image {
        id: volumeIndicator
        anchors.right: batteryIndicator.left
        source: "image://meegotheme/icons/statusbar/volume-muted"

        property variant controller: null
        property bool active: container.active

        function updateVolumeGraphic() {
            if (controller.mute)
            {
                source = "image://meegotheme/icons/statusbar/volume-muted";
            }
            else
            {
                if (controller.volume > 66)
                {
                    source = "image://meegotheme/icons/statusbar/volume-high";
                }
                else if (controller.volume > 33)
                {
                    source = "image://meegotheme/icons/statusbar/volume-medium";
                }
                else
                {
                    source = "image://meegotheme/icons/statusbar/volume-low";
                }
            }
        }

        onActiveChanged: {
            if (active)
            {
                controller = volumeControlComponent.createObject(volumeIndicator);
                updateVolumeGraphic();
            }
            else
            {
                controller.destroy();
            }
        }

        Component {
            id: volumeControlComponent
            VolumeControl {
                onMute: {
                    volumeIndicator.updateVolumeGraphic();
                }
                onVolumeChanged: {
                    volumeIndicator.updateVolumeGraphic();
                }
            }
        }
    }

    Image {
      id:musicPlayingIcon
      anchors.right: notificationIndicator.left
      source: "image://meegotheme/icons/actionbar/media-play" 
      height: volumeIndicator.paintedHeight
      width: volumeIndicator.paintedWidth     
      visible: musicIndicator.state == "playing" 
    }

    MusicIndicator {
      id: musicIndicator	
       
      onStateChanged: {
	musicPlayingIcon.visible = state == "playing" ? 1 : 0;
      }
    }

    BatteryIndicator {
        id: batteryIndicator
        anchors.right: parent.right
        active: container.active
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

        onReleased: {
            if( mouseArea.mouseY - mouseArea.firstY > 10)
            {
                mainWindow.triggerSystemUIMenu()
                triggered = true
            }
        }
    }
}
