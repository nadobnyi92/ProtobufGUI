#include "prototypedialog.h"
#include "protomanager.h"

#include "ui_prototypedialog.h"

ProtoTypeDialog::ProtoTypeDialog(const ProtoManager& manager, QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::ProtoTypeDialog)
{
    ui->setupUi(this);

    mClasses = manager.getProtoClasses();
    ui->cbPackage->addItems(removeEmptyOrDupl(mClasses.keys()));

    connect(ui->cbPackage, SIGNAL(currentTextChanged(const QString&)),
            SLOT(onSetPackage(const QString&)));
}

void ProtoTypeDialog::onSetPackage(const QString &package)
{
    ui->cbClass->clear();
    ui->cbClass->addItems(removeEmptyOrDupl(mClasses.values(package)));
}

QString ProtoTypeDialog::pPackage() const
{
    return ui->cbPackage->currentText();
}

QString ProtoTypeDialog::pClass() const
{
    return ui->cbClass->currentText();
}
