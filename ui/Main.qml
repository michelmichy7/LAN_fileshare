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

    Loader {
        id: loader
        anchors.fill: parent
        z: 3
    }

    Rectangle {
        width: 100
        height: 100
        anchors.centerIn: parent
        color: "#141414"



        Column {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 40

            Text {
                       text: "How do you want to transfer?"
                       color: "white"
                       anchors.horizontalCenter: parent.horizontalCenter
                   }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                background: Rectangle {
                    anchors.centerIn: parent
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
                            backend.sendPacket()
                        loader.source = "Send_UI.qml"
                        }
                    }
                }
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                background: Rectangle {
                    anchors.centerIn: parent
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
                        onClicked: { backend.catchPacket()
                        loader.source = "Receive_UI.qml"
                    }
                        }
                }
            }
        }
    }
}
