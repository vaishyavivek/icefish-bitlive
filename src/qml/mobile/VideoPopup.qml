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
