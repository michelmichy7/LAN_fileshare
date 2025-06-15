import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import Manager 1.0

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
    Backend {
        id: backend
    }

    Rectangle {
    anchors.centerIn: parent
        Rectangle {
            width: 400; height: 275;
            anchors.centerIn: parent
        }
    }
}
