import QtQuick 2.15
import QtQuick.Controls 2.15

import "../custom"

Dialog {
    id: shareDialogBox
    width: parent.width*0.8
    height: 250
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
    anchors.centerIn: parent

    Rectangle {
        id: pageOne
        anchors.fill: parent

        Column {
            anchors.fill: parent
            spacing: 10

            Text {
                id: name
                width: parent.width
                text: myid.text.length === 0 ? qsTr("Obtaining ID from Bitlive Tracker") : qsTr("Give your Peers these credential to start a Meeting")
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: myidText
                width: parent.width
                text: qsTr("Your Seed ID")
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: myid
                width: parent.width
                text: backend.MyId
                font.family: "Arial Black"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
            }

            Text {
                id: passwordText
                width: parent.width
                text: qsTr("Your Seed Password")
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: password
                width: parent.width
                text: backend.Password
                font.family: "Arial Black"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
            }

            ZButton {
                width: 50
                height: 50
                anchors.horizontalCenter: parent.horizontalCenter
                iconColor: "red"
            }
        }
    }
}
