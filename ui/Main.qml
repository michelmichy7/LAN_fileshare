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
    anchors.centerIn: parent
    Rectangle {
        width: 200; height: 150;
        radius: 20
        anchors.centerIn: parent
            Grid {
                spacing: 20
                anchors.centerIn: parent
                Repeater {
                    model: listModel
                    delegate: Button {
                        width: 200; height: 150
                        background: Rectangle {
                            anchors.fill: parent
                            color: "grey"
                        }

                        ComputerIcon {
                            anchors.centerIn: parent
                            z: 3
                        }
                    }
                }
            }
        }
    }
}
