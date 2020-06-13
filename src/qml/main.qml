import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Column {
        anchors.fill: parent
        spacing: 10

        TextInput {
            id: username
            width: parent.width
        }

        TextInput {
            id: password
            width: parent.width
        }

        Button {
            id: btn
            text: "Connect"
            onClicked: socket.initiateConnection(username.text, password.text)
        }

        TextInput {
            width: parent.width
            onAccepted: socket.sendData(text)
        }

        Text {
            id: name
            text: socket.DebugMessage
            width: parent.width
        }
    }

}
