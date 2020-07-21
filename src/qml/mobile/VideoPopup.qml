import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia 5.15

Popup {
    id: videoPopup
    padding: 0
    closePolicy: Popup.NoAutoClose

    background: Rectangle {

        color: "palegreen"
        opacity: 0.5
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        radius: 5
        border.width: 1
        border.color: "green"
    }


    VideoOutput {
        id: myVideo
        width: parent.width
        height: parent.height
        fillMode: VideoOutput.PreserveAspectCrop
        orientation: backend.MyFeed.CameraRotation
        source: backend.MyFeed
    }
}
