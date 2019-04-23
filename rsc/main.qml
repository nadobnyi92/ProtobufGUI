import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.0

Window {
    property int toolHeight: 20

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
            text: "Пакет: "
            anchors.left: parent.left
        }
        ComboBox {
            id: cbPackage
            height: toolHeight
            anchors.left: lbPackage.right
            width: (parent.width - (lbClass.width + lbPackage.width + tbOpen.width) ) / 2
        }
        Label {
            id: lbClass
            text: "Класс: "
            anchors.left: cbPackage.right
        }
        ComboBox {
            id: cbClass
            height: toolHeight
            anchors.left: lbClass.right
            width: (parent.width - (lbClass.width + lbPackage.width + tbOpen.width) ) / 2
        }
        ToolButton {
            id: tbOpen
            text: "..."
            height: toolHeight
            width: toolHeight
            anchors.right: parent.right
        }
    }

    ListView {
        id: tvProtoData
        height: parent.height - 2 * toolHeight
        width: parent.width
        anchors.left: parent.left
        anchors.top: tbTop.bottom
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
            anchors.right: pbDataSave.left
        }
        Button {
            id: pbDataSave
            text: "Сохранить"
            height: toolHeight
            anchors.right: pbDataReset.left
        }
        Button {
            id: pbDataReset
            text: "Отменить"
            height: toolHeight
            anchors.right: parent.right
        }
    }

        /*
        RowLayout {
            id: topRow
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.left: parent.left
            height: toolHeight

            Label {
                text: "Пакет: "
            }
            ComboBox {
                id: cbPacket
                height: 20
            }
            Label {
                text: "Класс: "
            }
            ComboBox {
                id: cbClass
                height: 20
            }

            ToolButton {
                id: tbLoad
                height: 20
            }
        }

        ListView {
            id: lvProtoTree
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: topRow.bottom
            anchors.bottom: bottomRow.top
            height: parent.height - buttonHeight * 2
        }

        RowLayout {
            id: bottomRow
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: buttonHeight
        }
    }
    */
}
