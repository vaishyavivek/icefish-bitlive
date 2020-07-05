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
//            source: backend.MyFeed
            width: parent.width
            height: parent.height*0.6
        }


        SwipeView {
            id: meetingTokenSwipeView

            width: parent.width
            height: parent.width*0.3

            currentIndex: 0

            Rectangle {
                id: pageOne

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

                    Text {
                        id: swipeToRight
                        width: parent.width
                        text: qsTr("Swipe Right to join a party >>>")
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }

            Rectangle {
                id: pageTwo

                Column {
                    anchors.fill: parent
                    spacing: 10

                    ZTextField {
                        id: peerId
                        width: 200
                        placeholderText: "Peer ID"
                        anchors.horizontalCenter: parent.horizontalCenter
                        onAccepted: peerPassword.focus
                    }

                    ZTextField {
                        id: peerPassword
                        width: 200
                        placeholderText: "Peer Password"
                        anchors.horizontalCenter: parent.horizontalCenter
                        onAccepted: backend.initiateConnection(peerId.text, text)
                    }

                    ZButton {
                        id: initiateConnectionBtn
                        width: 100
                        text: "Connect"
                        anchors.horizontalCenter: parent.horizontalCenter
                        onClicked: {

                            backend.initiateConnection(peerId.text, peerPassword.text)
                            myfeed.source = null
                            mainSwipeView.currentIndex = 1
                        }
                    }

                    Text {
                        id: swipeToLeft
                        width: parent.width
                        text: qsTr("<<< Swipe Left to let others join your party")
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
        }
    }
}
