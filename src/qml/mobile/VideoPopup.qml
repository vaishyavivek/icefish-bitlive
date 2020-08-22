/* videoPopup.qml
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

Popup {
    id: videoPopup
    padding: 0
    closePolicy: Popup.NoAutoClose

    background: Rectangle {

        color: "grey"
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        radius: 5
        border.width: 1
        border.color: "black"
    }


    VideoOutput {
        id: myVideo
        width: parent.width
        height: parent.height
        fillMode: VideoOutput.PreserveAspectCrop
        orientation: backend.MyFeed.CameraRotation
        source: backend.MyFeed
    }

    ZComboBox {
        model: [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]
        width: mainWindow.width*0.25
        displayText: currentText + "%"
        currentIndex: backend.MyFeed.QualityBar
        onCurrentIndexChanged: backend.MyFeed.QualityBar = currentIndex
    }
}
