#ifndef PROTOVIEW_H
#define PROTOVIEW_H

#include <QDialog>

#include <google/protobuf/dynamic_message.h>

namespace Ui
{
    class ProtoView;
}

class ProtoView : public QDialog
{
    Q_OBJECT
public:
    ProtoView(google::protobuf::Message * msg, QWidget * parent = nullptr);

private:
    void displayText();
    void displayBin();
    void displayHex();

private:
    Ui::ProtoView *ui;
    google::protobuf::Message *mMessage;
};

#endif // PROTOVIEW_H
