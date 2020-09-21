#include <iostream>
#include <QApplication>
#include <QQmlApplicationEngine>

#include "tree_items/prototreeitem.h"
#include "protomanager.h"

int main( int argc, char* argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<ProtoManager>("qt.test", 1, 0, "ProtoManager");
    qmlRegisterType<ProtoTreeItem>("qt.test", 1, 0, "ProtoTreeItem");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
