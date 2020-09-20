import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

import QtQuick.Dialogs 1.2

import qt.test 1.0

Window {
    visible: true
    width: 480
    height: 640
    title: qsTr("ProtobufGUI")

    ProtoManager {
        id: protoManager
    }

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
            Layout.fillHeight: true
            Layout.fillWidth: true
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
            Layout.fillHeight: true
            model: protoManager.protoClasses
            onCurrentTextChanged: protoManager.curClass = currentText
        }
        ToolButton {
            id: btSelect
            Layout.fillHeight: true
            onClicked: fileDialog.open()
        }
        ToolButton {
            id: btTest
            onClicked: {
                selectClassDialog.open(0)
            }
        }
    }

    ListView {
        anchors.top: rTopPanel.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: rBottomPanel.top
        model: protoManager.rootItem
        TreeItem {
            id: rootItem 
        }
    }

    Row {
        id: rBottomPanel
        spacing: 5
        layoutDirection: Qt.RightToLeft
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        Button {
            id: btReset
            text: "Отменить"
        }
        Button {
            id: btSave
            text: "Сохранить"
        }
        Button {
            id: btLoad
            text: "Загрузить"
        }
    }

    ProtoFileDialog {
        id: fileDialog
        onAccepted: {
            protoManager.load(fileDialog.folder)
            selectClassDialog.protoManager = protoManager.clone()
        }
    }

    ProtoSelectDialog {
        id: selectClassDialog
    }
}
