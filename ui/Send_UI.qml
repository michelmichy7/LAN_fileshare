import QtQuick 2.15
import QtQuick.Controls.Basic

import "Icons/qml"

Rectangle {
    color: "#141414"
        anchors.centerIn: parent
        Rectangle {
            width: 460; height: 300;
            radius: 20
            anchors.centerIn: parent
                Grid {
                    spacing: 20
                    anchors.centerIn: parent
                    anchors.fill: parent
                    Repeater {
                        model: backend.model
                        delegate: Rectangle {
                            width: 100; height: 100
                                radius: 20
                                color: "grey"

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        backend.model.handleDevClick(index) }
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
