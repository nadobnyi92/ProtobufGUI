#include "MainWindow.h"
#include "WindowController.h"

MainWindow::MainWindow(QObject * parent) noexcept
    : QObject(parent)
{
    qmlRegisterType<WindowController>("com.protobufgui", 1, 0, "WindowController");
    mEngine.load(QUrl("qrc:/main.qml"));
}

