import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

import QtQuick.Dialogs 1.2

FileDialog {
    id: protoFileDialog
    title: "Please choose a file"
    folder: shortcuts.home
    selectFolder: true
}
