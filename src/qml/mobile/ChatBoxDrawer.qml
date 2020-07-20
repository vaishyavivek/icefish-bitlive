import QtQuick 2.15
import QtQuick.Controls 2.15

import "../custom"

Drawer {
    id: chatBoxDrawer
    width: parent.width*0.7
    height: parent.height
    edge: Qt.RightEdge
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
}
