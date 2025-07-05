import QtQuick 2.15
import QtQuick.Controls.Basic

import "Icons/qml"

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
                            backend.model.handleDevClick(index)
                            console.log("Clicked IP:", model.display)
                            backend.catchPacket()
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
