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
    property string conState: ""

    Connections {
        target: backend.udpManager
        function onShowConnectionPage(message) {
            console.log("QML ConPage");
            senderRequest = message
            overlay.source = "ConnectionRequest.qml"
        }

}
    Component.onCompleted: {
        console.log("Initial state:", backend.connectionState); // This forces QML to track it
    }

    Connections {
        target: backend
        function onConnectionStateChanged() {
            conState = backend.connectionState
            console.log("New state:", conState)


            if (conState === "5") {
                loader.source = ""
                loader.source = "FileShare.qml"
            }
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
                backend.setConnectionState(StatusClass.CONNECTION_APPROVED)
                overlay.source = ""

                //if (backend.connectionState === StatusClass.CONNECTION_APPROVED) {
                        //loader.source = "ConnectionRequest.qml"
                //}
            })
            item.doConnection.connect(function() {
                backend.tcpConnection_REC(senderRequest)
                })
        }
    }




    Rectangle {
        color: "#141414"
            anchors.centerIn: parent
            Rectangle {
                color: "#202020"
                width: 460; height: 300;
                radius: 20
                anchors.centerIn: parent
                GridView {
                    anchors.fill: parent
                    anchors.margins: 20
                    cellWidth: 120
                    cellHeight: 120
                    model: backend.model

                    delegate: Rectangle {
                        width: 100
                        height: 100
                        radius: 20
                        color: "#333333"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                backend.theirIp = model.display
                                backend.model.handleDevClick(index)
                                console.log("Clicked IP:", model.display)
                                backend.setConnectionState(StatusClass.REQUESTING_FOR_CONNECTION)
                            }
                        }

                        ComputerIcon {
                            anchors.centerIn: parent
                            anchors.topMargin: 10
                            z: 3
                        }

                        Text {
                            text: model.display
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.bottomMargin: 10
                            color: "white"
                        }
                    }
                }
        }
    }
}
/* OLD UI
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
*/
