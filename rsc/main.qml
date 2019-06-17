import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.0
import QtQuick.Dialogs 1.3
import com.protobufgui 1.0

Window {
    property int toolHeight: 30

    id: window
    visible: true
    title: qsTr("ProtobufGUI")
    width: 400
    height: 600

    ToolBar {
        id: tbTop
        height: toolHeight
        width: parent.width

        Label {
            id: lbPackage
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            text: "Пакет: "
            anchors.left: parent.left         
        }
        ComboBox {
            id: cbPackage
            height: toolHeight
            anchors.left: lbPackage.right
            width: (parent.width - (lbClass.width + lbPackage.width + tbOpen.width) ) / 2
            model: controller.packages
            onCurrentTextChanged: controller.curPackage = cbPackage.currentText
        }
        Label {
            id: lbClass
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            text: "Класс: "
            anchors.left: cbPackage.right
        }
        ComboBox {
            id: cbClass
            height: toolHeight
            anchors.left: lbClass.right
            width: (parent.width - (lbClass.width + lbPackage.width + tbOpen.width) ) / 2
            model: controller.classes
            onCurrentTextChanged: controller.curClass = cbClass.currentText
        }
        ToolButton {
            id: tbOpen
            text: "..."
            height: toolHeight
            width: toolHeight
            anchors.right: parent.right
            onClicked: {
                protoFolderDialog.open()
            }
        }
    }

    TreeView {
        id: tvProtoData
        height: parent.height - 2 * toolHeight
        width: parent.width
        anchors.left: parent.left
        anchors.top: tbTop.bottom
        headerVisible: true

        TableViewColumn {
            role: "name"
            title: "Поле"
        }
        TableViewColumn {
            role: "type"
            title: "Тип"
        }
        TableViewColumn {
            role: "requirment"
            title: "Признак обязаельности"
        }
        TableViewColumn {
            role: "value"
            title: "Значение"
        }
    }

    ToolBar {
        id: tbBottom
        height: toolHeight
        width: parent.width
        anchors.top: tvProtoData.bottom
        anchors.left: parent.left

        Button {
            id: pbDataLoad
            text: "Загрузить"
            height: toolHeight
            width: parent.width / 3
            anchors.right: pbDataSave.left
        }
        Button {
            id: pbDataSave
            text: "Сохранить"
            height: toolHeight
            width: parent.width / 3
            anchors.right: pbDataReset.left
        }
        Button {
            id: pbDataReset
            text: "Отменить"
            height: toolHeight
            width: parent.width / 3
            anchors.right: parent.right
        }
    }

    WindowController {
        id:controller
        onProtoTreeModelChange: {
            tvProtoData.model = controller.model
        }
    }

    FileDialog {
        id: protoFolderDialog
        title: "Select folder with proto files"
        selectFolder: true
        folder: shortcuts.home
        onAccepted: {
            controller.loadProtoClasses(protoFolderDialog.fileUrl)
        }
        onRejected: {
            console.log("Canceled")
        }
        Component.onCompleted: visible = false
    }
}
