/* main.qml
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
import QtQuick.Window 2.15

Window {
    id: mainWindow
    visible: true
    width: 480
    height: 854

    title: qsTr("Icefish Bitlive")


    VideoPopup {
        id: myVideoPopup
        width: parent.width
        height: parent.height
        x: 0
        y: 0
    }

    BottomDrawer {
        id: bottomDrawer
    }


    MainRoom {
        id: mainRoom
        width: parent.width
        height: parent.height
        visible: false
    }


    Connections {
        target: backend
        function onSwitchToMainRoomView() {

            changeToMainRoom()
        }
    }

    function changeToMainRoom() {

        bottomDrawer.close()

        myVideoPopup.height = mainWindow.height*0.25
        myVideoPopup.width = mainWindow.width*0.25
        myVideoPopup.x = mainWindow.x + mainWindow.width - width

        mainRoom.visible = true
    }


    Component.onCompleted: {
        myVideoPopup.open()
        bottomDrawer.open()
    }
}
