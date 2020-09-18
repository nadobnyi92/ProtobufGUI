import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

import QtQuick.Dialogs 1.2

import qt.test 1.0

Dialog {
    property var protoManager: manager

    RowLayout {
        id: rTopPanel
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 5
        Label {
            id: lblPackage
            Layout.alignment: Qt.AlignVCenter
            text: "Пакет: "
        }
        ComboBox {
            id: cbPackage
            Layout.fillWidth: true
            height: 30
            model: protoManager.protoPackages
            onCurrentTextChanged: protoManager.curPackage = currentText
        }
        Label {
            id: lblClass
            Layout.alignment: Qt.AlignVCenter
            text: "Класс: "
        }
        ComboBox {
            id: cbClass
            Layout.fillWidth: true
            height: 30
            model: protoManager.protoClasses
            onCurrentTextChanged: protoManager.curClass = currentText
        }
    }
}
