#include "MainWindow.h"

MainWindow::MainWindow(QObject * parent) noexcept
    : QObject(parent)
{
    mEngine.load(QUrl("qrc:/main.qml"));
}

