#include <iostream>

#include <QFileDialog>
#include <QMap>

#include "MainWindow.h"
#include "ProtoManager.h"
#include "fielddelegate.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget * parent) noexcept
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tvProtoTree->setModel(&mModel);

    auto header = ui->tvProtoTree->header();

    header->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(ui->tbLoad, SIGNAL(clicked()), SLOT(onLoadClasses()));
    connect(ui->cbPackage, SIGNAL(currentTextChanged(const QString&)),
            &mProtoManager, SLOT(setPackage(const QString&)));
    connect(ui->cbClass, SIGNAL(currentTextChanged(const QString&)),
            &mProtoManager, SLOT(setClass(const QString&)));
    connect(&mProtoManager, SIGNAL(onProtoChange(const QStringList&)),
            SLOT(onSetPackages(const QStringList&)));
    connect(&mProtoManager, SIGNAL(onPackageChange(const QStringList&)),
            SLOT(onSetClasses(const QStringList&)));
    connect(&mProtoManager, SIGNAL(onClassChange(const proto::Descriptor*)),
            &mModel, SLOT(setProtoClass(const proto::Descriptor *)));
    connect(ui->tvProtoTree, SIGNAL(expanded(const QModelIndex&)),
            &mModel, SLOT(onExpand(const QModelIndex&)));

    ui->tvProtoTree->setItemDelegateForColumn(ProtobufModel::DATA_COLUMN, new FieldDelegate(this));
}

void MainWindow::onLoadClasses()
{
    QUrl path = QFileDialog::getExistingDirectoryUrl();
    if(!path.isEmpty())
    {
         mProtoManager.load(path);
    }
}

void MainWindow::onSetPackages(const QStringList& packages)
{
    ui->cbPackage->clear();
    ui->cbPackage->addItems(packages);
}

void MainWindow::onSetClasses(const QStringList& classes)
{
    ui->cbClass->clear();
    ui->cbClass->addItems(classes);
}
