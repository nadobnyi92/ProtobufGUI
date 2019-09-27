#include "prototypedialog.h"
#include "protomanager.h"

#include "ui_prototypedialog.h"

ProtoTypeDialog::ProtoTypeDialog(const ProtoManager& manager, QWidget * parent)
    : QDialog(parent), ui(new Ui::ProtoTypeDialog), mManger(manager)
{
    connect(ui->cbPackage, SIGNAL(currentTextChanged(const QString&)),
            &mManger, SLOT(setPackage(const QString&)));
    connect(&mManger, SIGNAL(onPackageChange(const QStringList&)),
            SLOT(onSetClasses(const QStringList&)));

    ui->cbPackage->addItems(mManger.getPackages());
}

void ProtoTypeDialog::onSetPackages(const QStringList & items)
{
    ui->cbPackage->clear();
    ui->cbPackage->addItems(items);
}

void ProtoTypeDialog::onSetClasses(const QStringList &items)
{
    ui->cbClass->clear();
    ui->cbClass->addItems(items);
}
