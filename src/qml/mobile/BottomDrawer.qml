/* BottomDrawer.qml
 * IceFish bitLive aims to solve end to end connectivity issue
 * without any dependency on a centralized server
 * by using a complete P2P driven approach.
 * © IceFish Community, 2020
 *
 * This file is part of IceFish bitLive.
 *
 * IceFish bitLive is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * IceFish bitLive is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with IceFish bitLive.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This project is hosted here,
 * https://github.com/vaishyavivek/icefish-bitlive
*/
import QtQuick 2.15
import QtQuick.Controls 2.15

import "../custom"

Drawer {
    id: bottomDrawer
    width: parent.width
    height: parent.height*0.45
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
                onClicked: backend.IsAudioRunning = !checked
                checked: backend.IsAudioRunning

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
                onClicked: backend.IsVideoRunning = !checked
                checked: backend.IsVideoRunning

            }

        }


        Column {
            id: mainContentJoin
            width: background.width*0.9
            height: background.height - background.width*0.1
            anchors.centerIn: parent
            spacing: background.width*0.05

            Row {
                width: parent.width
                height: parent.height*0.5 - parent.spacing
                spacing: parent.spacing

                Column {
                    width: parent.width*0.8 - parent.spacing
                    height: parent.height
                    spacing: parent.spacing

                    ZTextField {
                        id: peerName
                        width: parent.width
                        height: parent.height*0.5
                        placeholderText: "Your Name"
                        onAccepted: peerId.focus
                        onTextChanged: {

                            errored = text.length < 3
                        }
                    }


                    ZTextField {
                        id: seedId
                        width: parent.width
                        height: parent.height*0.5
                        placeholderText: "Seed Room Id"
                        onAccepted: sendDataToBackend()
                        onTextChanged: {

                            if (text.length > 3 && text.charAt(3) != "-")
                                text = text.slice(0, 3) + "-" + text.slice(3)

                            if (text.length >7 && text.charAt(7) != "-")
                                text = text.slice(0, 7) + "-" +text.slice(7)

                            errored = text.length < 12
                        }
                    }
                }

                ZButton {
                    id: initiateConnectionBtn
                    topPadding: parent.spacing
                    width: parent.width*0.2
                    height: width
                    mainIcon: "/main/src/assets/join_Meeting.png"
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: sendDataToBackend()
                }
            }


            ZButton {
                id: createMeetingPageSwipeBtn
                topPadding: parent.spacing
                height: parent.height*0.5
                width: height
                anchors.horizontalCenter: parent.horizontalCenter
                mainIcon: "/main/src/assets/new_Meeting.png"
                onClicked: createRoom()
            }
        }
    }


    function sendDataToBackend() {

        if (peerName.text.length < 3) {

            peerName.focus = true
            peerName.errored = true

            if (seedId.text.length < 12)
                seedId.errored = true
        }
        else if (seedId.text.length < 12) {

            seedId.focus = true
            seedId.errored = true
        }
        else {

            backend.MyFeed.Username = peerName.text
            backend.initiateConnection(seedId.text)
        }
    }


    function createRoom() {

        if (peerName.text.length < 3) {

            peerName.focus = true
            peerName.errored = true
        }
        else {

            backend.MyFeed.Username = peerName.text
            changeToMainRoom()
            mainRoom.openShareInfo()
        }
    }

    onOpened: peerName.focus = true
}
