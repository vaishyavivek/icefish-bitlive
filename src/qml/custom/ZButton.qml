import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0

Button {
    id: control
    text: qsTr("Button")
    property string iconColor: "grey"

    contentItem: Rectangle{
        width: control.width
        height: control.height
        color: "transparent"
        Image{
            id: image
            visible: false
            source: icon.source
            sourceSize.width: icon.width
            sourceSize.height: icon.height
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent
        }

        ColorOverlay{
            anchors.fill: image
            source: image
            color: iconColor
        }
    }

    background: Rectangle {
        implicitWidth: control.width
        implicitHeight: control.height
        opacity: enabled ? 1 : 0.3
//        border.color: control.down ? "#17a81a" : "#21be2b"
//        border.width: 1
        radius: height*0.5
    }
}
