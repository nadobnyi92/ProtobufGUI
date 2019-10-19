#ifndef PROTOBUFGUI_MAINWINDOW_H
#define PROTOBUFGUI_MAINWINDOW_H

#include <QObject>
#include <QMainWindow>

#include "protomanager.h"
#include "protobufmodel.h"

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
    void onPrepareMenu( const QPoint& p );

    void onAddItem();
    void onRemoveItem();
    void onReplaceType();

    void onSaveProtoData();

private:
    Ui::MainWindow * ui;
    ProtoManager mProtoManager;
    ProtobufModel mModel;

    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event) override;
};


#endif //PROTOBUFGUI_MAINWINDOW_H
