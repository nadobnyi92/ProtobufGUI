import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.12

//import qt.test 1.0

Column {
    property bool expanded: false
    leftPadding: 15
    id: treeNode
    Label {
        text: title()
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if(modelData.children.length)
                    treeNode.expanded = !treeNode.expanded
            }
        }
    }
    Column {
        Loader { source: expanded ? "TreeItem.qml" : "EmptyItem.qml" }
    }

    function title() {
        if(modelData.children.length > 0) {
            return (expanded ? " + " : " - ") + modelData.name
        } else {
            return modelData.name
        }
    }
}
