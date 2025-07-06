import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import "Icons/qml"

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Nearby File Sender")
    color: "#141414"

    property string senderRequest: ""

    Connections {
        target: backend
        function onShowConnectionPage(message) {
            senderRequest = message
            loader.source = "ConnectionRequest.qml"
            Console.log("Connection emit received on qml: ", message)
        }
    }

    Loader {
        id: loader
        anchors.fill: parent
        z: 3
        onLoaded: {
            if (loader.item && loader.item.hasOwnProperty("senderRequest")) {
                loader.item.senderRequest = senderRequest
            }
        }
    }

    Rectangle {
        width: 100
        height: 100
        anchors.centerIn: parent
        color: "#141414"

        Column {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 15

            Text {
                text: "How do you want to transfer?"
                color: "white"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 95
                height: 40
                radius: 15
                color: "white"
                Text {
                    text: "Send"
                    color: "black"
                    anchors.centerIn: parent
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        backend.catchPacket()
                        loader.source = "Send_UI.qml"
                    }
                }
            }

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 95
                height: 40
                radius: 15
                color: "white"
                Text {
                    text: "Receive"
                    color: "black"
                    anchors.centerIn: parent
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        backend.sendPacket()
                        backend.catchPacket()
                        loader.source = "Receive_UI.qml"
                    }
                }
            }
        }
    }
}
