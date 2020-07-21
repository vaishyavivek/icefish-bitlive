import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0

Button {
    id: control
    property string mainIcon: ""
    property string pressedIcon: mainIcon
    property string alterIcon: mainIcon

    text: qsTr("Button")
    display: AbstractButton.IconOnly
    checked: checkable

    contentItem: Rectangle{
        width: control.width
        height: control.height
        color: "transparent"

        Image{
            id: image
            source: (control.pressed ? pressedIcon : (control.checked ? alterIcon : mainIcon))
            sourceSize.width: icon.width
            sourceSize.height: icon.height
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent
        }
    }

    background: Rectangle {
        implicitWidth: control.width
        implicitHeight: control.height
        opacity: enabled ? 1 : 0.3
        color: "transparent"
        radius: height*0.5
    }

    onClicked: {
        checkable = !checkable
    }
}
