#ifndef PROTOBUFGUI_MAINWINDOW_H
#define PROTOBUFGUI_MAINWINDOW_H

#include <QObject>
#include <QtQml/QQmlApplicationEngine>

class MainWindow : public QObject{
    Q_OBJECT

public:
    MainWindow(QObject * parent = nullptr) noexcept;

private:
    QQmlApplicationEngine mEngine;
};


#endif //PROTOBUFGUI_MAINWINDOW_H
