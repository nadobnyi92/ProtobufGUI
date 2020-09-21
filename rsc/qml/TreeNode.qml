import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

//import qt.test 1.0

Column {
    id: treeNode
    property bool expanded: false
    RowLayout {
        spacing: 10
        Label {
            width: 10
            text: modelData.children.length > 0 ? (expanded ? " - " : " + ") : " "
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    modelData.expand()
                    treeNode.expanded = !treeNode.expanded
                }
            }
        }
        Label {
            width: 125
            text: modelData.name
        }
        Label {
            text: modelData.typeName
        }
    }
    Column {
        width: parent.width
        Loader { source: expanded ? "TreeItem.qml" : "EmptyItem.qml" }
    }
}


