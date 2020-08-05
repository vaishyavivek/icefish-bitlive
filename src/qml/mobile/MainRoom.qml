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
