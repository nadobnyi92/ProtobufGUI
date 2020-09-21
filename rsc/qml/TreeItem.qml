import QtQuick 2.12

Column {
    leftPadding: 15
    Repeater{
        model: modelData.children
        delegate: TreeNode{ }
    }
}
