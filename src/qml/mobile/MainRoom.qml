import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia 5.15


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
}
