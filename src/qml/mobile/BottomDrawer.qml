import QtQuick 2.15
import QtQuick.Controls 2.15

import "../custom"

Drawer {
    id: bottomDrawer
    width: parent.width
    height: parent.height*0.5
    edge: Qt.BottomEdge
    interactive: false

    background: Rectangle {

        color: "palegreen"
        opacity: 0.8
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
                id: muteMyBtnHome
                width: height
                height: parent.height
                icon.width: width
                icon.height: height
                mainIcon: "/main/src/assets/mic_Off.png"
                pressedIcon: "/main/src/assets/mic_Pressed.png"
                alterIcon: "/main/src/assets/mic_On.png"
                onClicked: backend.changeVocalSettings()

            }

            ZButton {
                id: hideMyVideoBtnHome
                width: height
                height: parent.height
                icon.width: width
                icon.height: height
                mainIcon: "/main/src/assets/camera_Off.png"
                pressedIcon: "/main/src/assets/camera_Pressed.png"
                alterIcon: "/main/src/assets/camera_On.png"
                onClicked: backend.changeFeedSettings()

            }

        }


        Column {
            width: parent.width*0.8
            height: parent.height*0.8
            anchors.centerIn: parent
            spacing: height*0.1

            Row {
                width: parent.width
                height: parent.height*0.5
                spacing: 15

                Column {
                    width: parent.width*0.8
                    height: parent.height
                    spacing: 15

                    ZTextField {
                        id: peerName
                        width: parent.width
                        height: parent.height*0.3
                        placeholderText: "Your Name (optional)"
                        onAccepted: peerId.focus
                    }

                    ZTextField {
                        id: peerId
                        width: parent.width
                        height: parent.height*0.3
                        placeholderText: "Peer ID"
                        onAccepted: peerPassword.focus
                    }

                    ZTextField {
                        id: peerPassword
                        width: parent.width
                        height: parent.height*0.3
                        placeholderText: "Peer Password"
                        onAccepted: backend.initiateConnection(peerId.text, text)
                    }
                }

                ZButton {
                    id: initiateConnectionBtn
                    width: parent.width*0.2 - 15
                    height: width
                    mainIcon: "/main/src/assets/join_Meeting.png"

                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: {

                        backend.initiateConnection(peerId.text, peerPassword.text)
                    }
                }
            }

            ZButton {
                id: createMeetingPageSwipeBtn
                height: parent.height*0.4
                width: height
                anchors.horizontalCenter: parent.horizontalCenter
                mainIcon: "/main/src/assets/new_Meeting.png"
                font.pointSize: 30
                font.bold: true
                onClicked: {

                    changeToMainRoom()
                    mainRoom.openShareInfo()
                }
            }

            ZButton {
                id: temoBtn

            }
        }
    }

}
