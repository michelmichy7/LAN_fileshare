import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    color: "#141414"

    Rectangle {
        anchors.centerIn: parent

     Column {
        anchors.centerIn: parent
        spacing: 25

            Button {
                text: "Send"
             }

            Button {
                text: "Claim"
            }
     }
    }
}
