/* MainRoom.qml
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
import QtMultimedia 5.15

import "../custom"

Rectangle {
    id: mainRoom
    width: parent.width
    height: parent.height
    color: "black"

    ListView {
        id: videoOutputListView
        width: parent.width
        height: parent.height
        model: backend.PeerFeedList

        delegate: Rectangle {
            width: parent.width
            height: parent.height

            VideoOutput {
                width: mainRoom.width
                height: mainRoom.height
                source: model.modelData
            }
        }
    }

    Popup {
        id: chatBoxDrawerBtn
        padding: 0
        closePolicy: Popup.NoAutoClose
        y: videoControlDrawer.y - 75
        x: parent.width - 75
        width: 70
        height: 70

        background: Rectangle {
            color: "transparent"
        }

        ZButton {
            id: openChatSideDrawer
            width: height
            height: parent.height
            icon.width: width
            icon.height: height
            mainIcon: "/main/src/assets/chat.png"
            onClicked: chatBoxDrawer.open()

        }
    }

    VideoControlDrawer {
        id: videoControlDrawer
    }

    ShareDialogBox {
        id: shareDialogBox
    }

    ChatBoxDrawer {
        id: chatBoxDrawer
    }

    function openShareInfo() {

        videoControlDrawer.open()
        shareDialogBox.open()
    }


    MouseArea {
        anchors.fill: parent
        onClicked: videoControlDrawer.open()

    }

    onVisibleChanged: {
        if (visible)
            chatBoxDrawerBtn.open()
    }
}
