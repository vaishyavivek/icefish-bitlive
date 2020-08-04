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
