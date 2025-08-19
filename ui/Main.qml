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
    property string actState: ""

    Connections {
        target: backend.udpManager
        function onShowConnectionPage(message) {
            console.log("QML ConPage");
            senderRequest = message
            overlay.source = "ConnectionRequest.qml" //DONE: on discard is needed to be changed state back to discovering
        }

}
   /*Component.onCompleted: {
        console.log("Initial state:", backend.connectionState); // This forces QML to track it
    }*/

    Connections {
        target: backend
        function onConnectionStateChanged() {
            conState = backend.connectionState
            console.log("New ConState:", conState)


            if (conState === "5") {
                loader.source = ""
                loader.source = "FileShare.qml"
            }
        }
        function onActivityStateChanged() {
            actState = backend.activityState
            console.log("New ActState:", actState)
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
                backend.setConnectionState(StatusClass.DISCOVERING_DEVICES)
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



    Column {
        spacing: 20
        anchors.centerIn: parent

        // Devices list
        Rectangle {
            color: "#141414"
            width: 500
            height: 350
            radius: 20

            Rectangle {
                color: "#202020"
                anchors.fill: parent
                anchors.margins: 20
                radius: 20

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

                        Column {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 5
                            anchors.centerIn: parent

                            ComputerIcon {
                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            Text {
                                text: model.display
                                color: "white"
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }
                }
            }
        }

        // Selected IP display
        Rectangle {
            width: 200
            height: 40
            radius: 10
            color: "#141414"
            anchors.horizontalCenter: parent.horizontalCenter


            Text {
                anchors.centerIn: parent
                text: "IP of this device: " + backend.localIp
                color: "white"
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
