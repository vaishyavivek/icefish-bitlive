/* shareDialogBox.qml
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

import com.lasconic 1.0
import "../custom"

Dialog {
    id: shareDialogBox
    width: parent.width*0.7
    height: 200
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
    anchors.centerIn: parent

    ShareUtils {
        id: shareUtils
    }

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
        id: pageOne
        anchors.fill: parent
        color: "transparent"

        Column {
            anchors.fill: parent
            anchors.margins: 10
            padding: 20
            spacing: 15

            Text {
                id: name
                width: parent.width
                text: "Meeting Details"
                font.bold: true
                font.pointSize: 16
                opacity: 0.8
            }

            Text {
                id: myidText
                width: parent.width
                text: qsTr("Seed Room ID: <br>" + backend.RoomId + (shareUtils.IsClipboardSet ? "   <i>Copied</i>" : ""))
                textFormat: Text.StyledText
                font.pointSize: 12
                opacity: 0.7
            }

            ZButton {
                width: 50
                height: 50
                anchors.horizontalCenter: parent.horizontalCenter
                mainIcon: "/main/src/assets/share.png"
                onClicked: {

                    shareUtils.share("Lets get live in bits", backend.RoomId)
                }
            }
        }
    }
}
