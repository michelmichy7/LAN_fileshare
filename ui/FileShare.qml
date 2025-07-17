import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    anchors.fill: parent
    color: "#141414"

    property var filePaths: []

    Column {
        anchors.centerIn: parent
        spacing: 20

        Rectangle {
            width: 450
            height: 250
            color: "#202020"
            radius: 35
            clip: true
            anchors.horizontalCenter: parent.horizontalCenter

            ListView {
                id: listView
                anchors.fill: parent
                anchors.margins: 20
                model: filePaths
                spacing: 5

                delegate: Rectangle {
                    width: listView.width
                    height: 30
                    color: "transparent"

                    Text {
                        anchors.fill: parent
                        anchors.margins: 10
                        text: modelData
                        color: "white"
                        font.pixelSize: 11
                        elide: Text.ElideMiddle
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        wrapMode: Text.NoWrap
                    }

                    Rectangle {
                        width: parent.width
                        height: 1
                        color: "#333333"
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }

                Text {
                    anchors.centerIn: parent
                    text: "No files selected"
                    color: "#666666"
                    font.pixelSize: 14
                    visible: filePaths.length === 0
                }

                ScrollBar.vertical: ScrollBar {
                    active: listView.moving || listView.flicking
                    policy: ScrollBar.AsNeeded
                    contentItem: Rectangle {
                        implicitWidth: 6
                        implicitHeight: 100
                        radius: width / 2
                        color: "#555555"
                    }
                }
            }
        }

        Rectangle {
            width: 120
            height: 35
            radius: 15
            color: mouseArea.pressed ? "#e0e0e0" : "white"
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: "Select Files"
                color: "black"
                anchors.centerIn: parent
                font.pixelSize: 12
            }

            MouseArea {
                id: selectArea
                anchors.fill: parent
                onClicked: {
                    let files = FileDialogHelper.openFileDialog()
                    filePaths = files
                }
            }
        }
        Rectangle {
            width: 120
            height: 35
            radius: 15
            color: mouseArea.pressed ? "#e0e0e0" : "white"
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: "Send Files"
                color: "black"
                anchors.centerIn: parent
                font.pixelSize: 12
            }

            MouseArea {
                id: sendArea
                anchors.fill: parent
                onClicked: {

                }
            }
        }
    }
}
