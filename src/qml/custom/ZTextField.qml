import QtQuick 2.12
import QtQuick.Controls 2.12

TextField {
    id: control
    placeholderText: qsTr("Enter description")

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 40
        color: control.enabled ? "transparent" : "#353637"
        border.color: control.enabled ? "#21be2b" : "transparent"
    }
}
