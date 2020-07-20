import QtQuick 2.15
import QtQuick.Controls 2.15

import "../custom"

Drawer {
    id: bottomDrawer
    width: parent.width
    height: parent.height*0.4
    edge: Qt.BottomEdge
    interactive: false

    background: Rectangle {

        color: "palegreen"
        opacity: 0.5
        width: parent.width*0.9
        height: parent.height*0.9
        anchors.centerIn: parent
        radius: 15
        border.width: 1
        border.color: "green"
    }

    Rectangle {
        id: joinRoomPage
        anchors.fill: parent
        color: "transparent"

        Row {
            id: extraBtns
            y: -60
            height: 60
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 15

            ZButton {
                id: muteMyBtn
                width: height
                height: parent.height
                icon.width: width
                icon.height: height
                icon.source: "/main/src/icons/mic.png"
                iconColor: "red"
            }

            ZButton {
                id: hideMyVideoBtn
                width: height
                height: parent.height
                icon.width: width
                icon.height: height
                icon.source: "/main/src/icons/video.png"

            }

        }


        Column {
            width: parent.width*0.8
            height: parent.height*0.8
            anchors.centerIn: parent
            spacing: height*0.1

            Row {
                width: parent.width
                height: parent.height*0.4
                spacing: 15

                Column {
                    width: parent.width*0.8
                    height: parent.height
                    spacing: 15

                    ZTextField {
                        id: peerId
                        width: parent.width
                        height: parent.height*0.45
                        placeholderText: "Peer ID"
                        onAccepted: peerPassword.focus
                    }

                    ZTextField {
                        id: peerPassword
                        width: parent.width
                        height: parent.height*0.45
                        placeholderText: "Peer Password"
                        onAccepted: backend.initiateConnection(peerId.text, text)
                    }
                }

                ZButton {
                    id: initiateConnectionBtn
                    width: parent.width*0.2 - 15
                    height: width
                    icon.width: width*0.4
                    icon.height: height*0.4
                    icon.source: "/main/src/icons/select.png"
                    iconColor: "green"

                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: {

                        backend.initiateConnection(peerId.text, peerPassword.text)
                    }
                }
            }

            ZButton {
                id: createMeetingPageSwipeBtn
                height: parent.height*0.5
                width: height
                anchors.horizontalCenter: parent.horizontalCenter
                icon.width: width*0.5
                icon.height: height*0.5
                icon.source: "/main/src/icons/add.png"
                iconColor: "green"
                font.pointSize: 30
                font.bold: true
                onClicked: {

                    changeToMainRoom()
                    mainRoom.openShareInfo()
                }
            }
        }
    }

}
