#include "protoview.h"
#include "ui_protoview.h"

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
    ui->textField->setText(mMessage->DebugString().c_str());
}

void ProtoView::displayBin()
{
    ui->textField->setText(mMessage->SerializeAsString().c_str());
}

void ProtoView::displayHex()
{
    std::string message = mMessage->SerializeAsString();
    QString hexMessage;
    for(auto x: message)
    {
        hexMessage += QString("0x%1 ").arg((int)x, 0, 16).rightJustified(3, '0').right(3);;
    }
    ui->textField->setText(hexMessage);
}
