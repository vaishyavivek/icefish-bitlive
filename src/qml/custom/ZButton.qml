/* ZButton.qml
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
import QtGraphicalEffects 1.0

Button {
    id: control
    property string mainIcon: ""
    property string pressedIcon: mainIcon
    property string alterIcon: mainIcon

    text: qsTr("Button")
//    display: AbstractButton.IconOnly
    checked: checkable

    contentItem: Rectangle{
        width: control.width
        height: control.height
        color: "transparent"

        Image {
            id: image
            source: (control.pressed ? pressedIcon : (control.checked ? mainIcon : alterIcon))
            width: parent.width
            height: width
            autoTransform: true
            mipmap: true
        }
    }

    background: Rectangle {
        implicitWidth: control.width
        implicitHeight: control.height
        opacity: enabled ? 1 : 0.3
        color: "transparent"
        radius: height*0.5
    }

    onClicked: {
        checkable = !checkable
    }
}
