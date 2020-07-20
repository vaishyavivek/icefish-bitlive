import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtMultimedia 5.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Icefish Bitlive")


    SwipeView {
        id: mainSwipeView

        currentIndex: 0
//        interactive: false
        anchors.fill: parent

        HomePage { }

        VideoOutputPage { }
    }
}
