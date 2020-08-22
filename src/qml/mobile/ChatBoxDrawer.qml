/* ChatBoxDrawer.qml
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
    id: chatBoxDrawer
    width: parent.width*0.7
    height: parent.height
    edge: Qt.RightEdge

    background: Rectangle {

        color: "palegreen"
        opacity: 0.5
        width: parent.width*0.99
        height: parent.height*0.99
        anchors.centerIn: parent
        radius: 5
        border.width: 1
        border.color: "green"
    }


    Rectangle {
        id: chatBox
        anchors.fill: parent
        color: "transparent"

        Column {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10

            ListView {
                width: parent.width
                height: parent.height - 65
                model: backend.ChatHistory
                spacing: 10

                delegate: Rectangle {
                    width: parent.width
                    height: textBox.height + 30
                    color: "transparent"

                    Row {
                        anchors.fill: parent
                        anchors.margins: 5
                        spacing: 10

                        Rectangle {
                            id: profilePicture
                            height: textBox.height - 10
                            width: height
                            color: "transparent"
                            visible: model.modelData.Direction

                            Image {
                                width: parent.width
                                height: width
                                autoTransform: true
                                mipmap: true
                                source: "/main/src/assets/default_User_Avatar_" + model.modelData.IconId + ".png"
                            }
                        }

                        Rectangle {
                            id: contentText
                            width: parent.width - textBox.height
                            height: parent.height
                            color: "transparent"

                            Column {
                                anchors.fill: parent
                                anchors.margins: 5
                                spacing: 5

                                Rectangle {
                                    width: parent.width
                                    height: 7
                                    color: "transparent"
                                    Text {
                                        width: parent.width
                                        height: parent.height
                                        opacity: 0.75
                                        text: qsTr(model.modelData.PeerName)
                                    }

                                }

                                Rectangle {
                                    width: parent.width
                                    height: parent.height - 24
                                    color: "transparent"

                                    Text {
                                        width: parent.width
                                        height: parent.height
                                        text: model.modelData.Text
                                        horizontalAlignment: model.modelData.Direction ? Text.AlignRight : Text.AlignLeft
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }

                                Rectangle {
                                    width: parent.width
                                    height: 7
                                    color: "transparent"
                                    Text {
                                        width: parent.width
                                        height: parent.height
                                        opacity: 0.75
                                        text: qsTr(model.modelData.TimeGenerated)
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignRight
                                    }

                                }
                            }


                            Rectangle {
                                anchors.fill: parent
                                z: -4
                                opacity: 0.1
                                color: model.modelData.Direction ? "blue" : "green"
                                radius: 5
                                border.width: 1
                                border.color: model.modelData.Direction ? "blue" : "green"
                            }
                        }

                        Rectangle {
                            id: profilePicture2
                            height: textBox.height - 10
                            width: height
                            color: "transparent"
                            visible: !model.modelData.Direction

                            Image {
                                width: parent.width
                                height: width
                                autoTransform: true
                                mipmap: true
                                source: "/main/src/assets/default_User_Avatar_" + model.modelData.IconId + ".png"
                            }
                        }
                    }
                }
            }

            Rectangle {
                id: textBoxParentRect
                width: parent.width
                height: 50
                color: "transparent"

                Row {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 5

                    ZTextField {
                        id: textBox
                        width: parent.width - height - spacing
                        height: parent.height
                        placeholderText: "Send a message"
                        onAccepted: sendText()
                    }

                    ZButton {
                        height: parent.height
                        width: height
                        anchors.verticalCenter: parent.verticalCenter
                        mainIcon: "/main/src/assets/join_Meeting.png"
                        onClicked: sendText()
                    }
                }
            }
        }
    }

    function sendText() {

        backend.sendText(textBox.text)
        textBox.clear()
    }

    onOpened: textBox.focus = true
}
