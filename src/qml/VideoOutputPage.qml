import QtQuick 2.12
import QtQuick.Controls 2.12
import QtMultimedia 5.12

import "custom"

Rectangle {
    color: "transparent"

    Column {
        anchors.fill: parent
        spacing: 10

        VideoOutput {
            id: myfeed
            source: backend.MyFeed
            width: parent.width
            height: parent.height*0.35
        }
        TextInput{
            width: parent.width
            height: 50
            onTextChanged: backend.sendText(text)
        }

        ListView {
            id: peerOutputList
            width: parent.width
            height: parent.height - myfeed.height - 10
            spacing: 5

            model: backend.PeerFeedList

            delegate: Rectangle {
                width: parent.width
                height: myfeed.height

                VideoOutput {
                    source: model.modelData
                    width: parent.width
                    height: parent.height
                }

                Text {
                    text: model.modelData.TextReceived
                }
            }

            onCountChanged: console.log(count)
        }
    }


}
