import QtQuick 2.15
import QtQuick.Controls 2.15

TextField {
    id: control
    placeholderText: qsTr("Enter description")

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 40
        color: control.enabled ? "transparent" : "#000000"
        border.color: control.enabled ? "palegrenn" : "transparent"
        radius: height*0.25
    }
}
