#ifndef PROTOBUFGUI_MAINWINDOW_H
#define PROTOBUFGUI_MAINWINDOW_H

#include <QObject>
#include <QMainWindow>

#include "ProtoManager.h"
#include "ProtobufModel.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget * parent = nullptr) noexcept;

public slots:
    void onLoadClasses();
    void onSetPackages(const QStringList&);
    void onSetClasses(const QStringList&);

private:
    Ui::MainWindow * ui;
    ProtoManager mProtoManager;
    ProtobufModel mModel;
};


#endif //PROTOBUFGUI_MAINWINDOW_H
