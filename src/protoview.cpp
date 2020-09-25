#include "protoview.h"
#include "ui_protoview.h"
#include "prototreeerror.h"

ProtoView::ProtoView(google::protobuf::Message *msg, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProtoView)
    , mMessage(msg)
{
    ui->setupUi(this);
    connect(ui->pbBinary, &QPushButton::clicked,
            [this]() {
                ui->pbBinary->setChecked(true);
                ui->pbText->setChecked(false);
                ui->pbHex->setChecked(false);
                displayBin();
            });
    connect(ui->pbText, &QPushButton::clicked,
            [this]() {
                ui->pbBinary->setChecked(false);
                ui->pbText->setChecked(true);
                ui->pbHex->setChecked(false);
                displayText();
            });
    connect(ui->pbHex, &QPushButton::clicked,
            [this]() {
                ui->pbBinary->setChecked(false);
                ui->pbText->setChecked(false);
                ui->pbHex->setChecked(true);
                displayHex();
            });
    displayText();
}

void ProtoView::displayText()
{
    try {
        ui->textField->setText(mMessage->DebugString().c_str());
    } catch (google::protobuf::FatalException& e) {
        throw ProtoTreeError("Failed get string message", e.message());
    }
}

void ProtoView::displayBin()
{
    try {
        ui->textField->setText(mMessage->SerializeAsString().c_str());
    } catch (google::protobuf::FatalException& e) {
        throw ProtoTreeError("Failed get string message", e.message());
    }
}

void ProtoView::displayHex()
{
    try {
        std::string message = mMessage->SerializeAsString();
        QString hexMessage;
        for(auto x: message)
        {
            hexMessage += QString("0x%1 ").arg((int)x, 0, 16).rightJustified(3, '0').right(3);;
        }
        ui->textField->setText(hexMessage);
    } catch (google::protobuf::FatalException& e) {
        throw ProtoTreeError("Failed get string message", e.message());
    }
}
