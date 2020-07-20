import QtQuick 2.15
import QtQuick.Controls 2.15

import "../custom"

Drawer {
    id: videoControlDrawer
    width: parent.width
    height: Math.max(parent.height*0.25, 200)
    edge: Qt.BottomEdge

    background: Rectangle {

        color: "palegreen"
        opacity: 0.5
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        radius: 5
//        border.width: 1
//        border.color: "green"
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"

        Column {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10

            Row {
                width: parent.width
                height: Math.max(parent.height*0.45, 70)
                spacing: 10

                ListView {
                    id: participantList
                    width: parent.width - height - 10
                    height: parent.height
                    orientation: ListView.Horizontal

                    delegate: Rectangle {

                        height: parent.height
                        width: height

                        Column {
                            anchors.fill: parent
                            spacing: 5

                            Image {
                                id: photoOfParticipant
                                width: parent.width*0.8
                                height: width
    //                            source: "file"
                            }

                            Text {
                                id: nameOfParticipant
                                text: model.modelData.PeerName
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: videoOutputListView.currentIndex = index
                        }
                    }
                }

                ZButton {
                    id: addNewParticipants
                    height: parent.height*0.9
                    width: height
                    anchors.verticalCenter: parent.verticalCenter
                    icon.width: width
                    icon.height: height
                    icon.source: "/main/src/icons/share.png"
                    iconColor: "transparent"
                    onClicked: shareDialogBox.open()
                }
            }


            Row {
                id: videoControlBtns
                height: Math.max(parent.height*0.45, 60)
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 15

                ZButton {
                    id: muteMyBtn
                    width: height
                    height: parent.height*0.9
                    icon.width: width
                    icon.height: height
                    icon.source: "/main/src/icons/mic.png"
                    iconColor: "transparent"
                }

                ZButton {
                    id: endCallBtn
                    width: height
                    height: parent.height
                    icon.width: width
                    icon.height: height
                    icon.source: "/main/src/icons/end.png"
                    iconColor: "transparent"
                }

                ZButton {
                    id: hideMyVideoBtn
                    width: height
                    height: parent.height*0.9
                    icon.width: width
                    icon.height: height
                    icon.source: "/main/src/icons/video.png"
                    iconColor: "transparent"
                }
            }
        }
    }
}
