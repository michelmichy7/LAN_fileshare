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
            width: 400; height: 275;
            anchors.centerIn: parent
            Grid {
                anchors.centerIn: parent
                ListView {
                    anchors.fill: parent
                    model: backend

                delegate: Rectangle {
                    width: 50; height: 50
                    color: "blue"
                    ComputerIcon {

                    }
                }
                }
            }
        }
    }
}
