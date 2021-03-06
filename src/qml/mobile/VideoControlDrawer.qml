/* videoControlDrawer.qml
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
    id: videoControlDrawer
    width: parent.width
    height: Math.max(parent.height*0.25, 200)
    edge: Qt.BottomEdge

    background: Rectangle {

        color: "palegreen"
        opacity: 0.5
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        radius: 5
    }



    Rectangle {
        anchors.fill: parent
        color: "transparent"

        Column {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10

            Row {
                width: parent.width
                height: Math.max(parent.height*0.45, 70)
                spacing: 10

                ListView {
                    id: participantList
                    width: parent.width - height - 10
                    height: parent.height
                    orientation: ListView.Horizontal
                    model: backend.PeerFeedList

                    delegate: Rectangle {

                        height: parent.height
                        width: height
                        color: "transparent"

                        Column {
                            anchors.fill: parent
                            spacing: 5

                            Image {
                                id: photoOfParticipant
                                width: parent.width*0.8
                                height: width
                                anchors.horizontalCenter: parent.horizontalCenter
                                source: "/main/src/assets/default_User_Avatar_" + Math.floor((Math.random() * 5) + 1) + ".png"
                                autoTransform: true
                                mipmap: true
                            }

                            Text {
                                id: nameOfParticipant
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: model.modelData.Username
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: videoOutputListView.currentIndex = index
                        }
                    }
                }

                ZButton {
                    id: addNewParticipants
                    height: parent.height*0.9
                    width: height
                    anchors.verticalCenter: parent.verticalCenter
                    mainIcon: "/main/src/assets/add_New_Member_To_Meeting.png"
                    onClicked: shareDialogBox.open()
                }
            }


            Row {
                id: videoControlBtns
                height: Math.max(parent.height*0.45, 60)
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 15

                ZButton {
                    id: muteMyBtn
                    width: height
                    height: parent.height*0.9
                    mainIcon: "/main/src/assets/mic_Off.png"
                    pressedIcon: "/main/src/assets/mic_Pressed.png"
                    alterIcon: "/main/src/assets/mic_On.png"
                    onClicked: backend.IsAudioRunning = !checked
                    checked: backend.IsAudioRunning
                }

                ZButton {
                    id: endCallBtn
                    width: height
                    height: parent.height
                    mainIcon: "/main/src/assets/close_Meeting.png"
                    pressedIcon: "/main/src/assets/cloese_Meeting_Pressed.png"
                    onClicked: Qt.quit()
                }

                ZButton {
                    id: hideMyVideoBtn
                    width: height
                    height: parent.height*0.9
                    mainIcon: "/main/src/assets/camera_Off.png"
                    pressedIcon: "/main/src/assets/camera_Pressed.png"
                    alterIcon: "/main/src/assets/camera_On.png"
                    onClicked: backend.IsVideoRunning = !checked
                    checked: backend.IsVideoRunning
                }
            }
        }
    }
}
