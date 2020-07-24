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
