import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import "Icons/qml"

import LAN.Backend 1.0


Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Nearby File Sender")
    color: "#141414"



    property string senderRequest: ""
    property int user //0 sender //1 receiver
/*
    Connections {
        target: udpManager
        function onShowConnectionPage(message) {
            senderRequest = message
            overlay.source = "ConnectionRequest.qml"
        }

        function onTcpConnected(ip) {
            loader.source = "FileShare.qml"
        }
    }

    Connections {
        target: udpManager
        function onShowConnectionPage(message) {
            senderRequest = message
            overlay.source = "ConnectionRequest.qml"
        }

        function onTcpConnected(ip) {
            loader.source = "FileShare.qml"
        }
    }*/


    Loader {
        id: overlay
        anchors.fill: parent
        z: 4
        onLoaded: {
            if (overlay.item && overlay.item.hasOwnProperty("senderRequest")) {
                overlay.item.senderRequest = senderRequest
            }

            item.requestDiscard.connect(function() {
                overlay.source = ""
            })

            item.requestAccept.connect(function() {
                overlay.source = ""

            })
            item.doConnection.connect(function() {
                backend.tcpConnection_REC(senderRequest)
                })
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
            item.requestDiscard.connect(function() {
                overlay.sourceComponent = "connectionState"
        })
            item.requestAccept.connect(function() {
                overlay.source = "FileShare.qml"
            })
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
                        backend.setConnectionState(StatusClass.DISCOVERING_DEVICES)

                        loader.source = "Send_UI.qml"
                        user = 0
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
                        backend.setConnectionState(StatusClass.TOLD_ABOUT_SELF)


                        loader.source = "Receive_UI.qml"
                        user = 1
                    }

                }
            }
        }
    }
}
