#include <iostream>
#include <fstream>

#include <QFileDialog>
#include <QMessageBox>
#include <QMap>
#include <QKeyEvent>
#include <QDebug>
#include <QMenu>

#include "mainwindow.h"
#include "protomanager.h"
#include "fielddelegate.h"

#include "prototypedialog.h"
#include "prototreeerror.h"
#include "protoview.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget * parent) noexcept
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tvProtoTree->setModel(&mModel);

    auto header = ui->tvProtoTree->header();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tvProtoTree->setItemDelegateForColumn(ProtobufModel::COL_VALUE, new FieldDelegate(this));
    ui->tvProtoTree->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->tvProtoTree->installEventFilter(this);

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
    connect(ui->pbSaveData, SIGNAL(clicked()), SLOT(onSaveProtoData()));
    connect(ui->pbLoadData, SIGNAL(clicked()), SLOT(onLoadProtoData()));
    connect(ui->pbReject, SIGNAL(clicked()), &mModel, SLOT(onClearData()));
    connect(ui->pbView, &QPushButton::clicked,
            [this](){
            });

    connect(ui->tvProtoTree, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(onPrepareMenu(const QPoint&)));

    connect(&mModel, SIGNAL(processProtoError(const ProtoTreeError&)),
            SLOT(onProcessProtoError(const ProtoTreeError&)));
}

void MainWindow::onLoadClasses()
{
    QUrl path = QFileDialog::getExistingDirectoryUrl(this, "load proto dirs", QDir::homePath());
    if(!path.isEmpty())
         mProtoManager.load(path);
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

void MainWindow::onPrepareMenu(const QPoint &p)
{
    QModelIndex idx = ui->tvProtoTree->indexAt(p);
    ProtoTreeItem *item = static_cast<ProtoTreeItem*>(idx.internalPointer());

//        if(item->type() == proto::FieldDescriptor::TYPE_BYTES) {
//            QAction * actTransform = new QAction("Преобразовать тип", &menu);
//            actTransform->setData(idx);
//            connect(actTransform, SIGNAL(triggered()), SLOT(onReplaceType()));
//            menu.addAction(actTransform);
//        }

    if(item->actions().size() > 0) {
        QMenu menu;
        menu.addActions(item->actions());
        menu.exec( ui->tvProtoTree->mapToGlobal(p) );
    }
}

void MainWindow::onReplaceType()
{
    ProtoTypeDialog dlg(mProtoManager, this);
    if(dlg.exec() == QDialog::Accepted) {
        QAction * act = static_cast<QAction*>(sender());
        QModelIndex idx = qvariant_cast<QModelIndex>(act->data());
        mModel.onReplaceType(idx,
            mProtoManager.getClassDescriptor(dlg.pPackage(), dlg.pClass()));
        ui->tvProtoTree->expand(idx.sibling(idx.row(),0));
    }
}

void MainWindow::onSaveProtoData()
{
    try {
        std::string message = mModel.getSerializedMessage();
        if(!message.empty()) {
            QString fp = QFileDialog::getSaveFileName(/*this, "save proto data", QDir::homePath()*/);
            std::ofstream of(fp.toStdString(), std::fstream::binary);
            if(of.is_open())
                of << message;
        }
    } catch (const ProtoTreeError& e) {
        onProcessProtoError(e);
    }
}

void MainWindow::onLoadProtoData()
{
    try {
        mModel.loadProtoData();
    } catch (const ProtoTreeError& e) {
        onProcessProtoError(e);
    }
}

void MainWindow::onViewProtoMessage()
{
    try {
        if(mModel.getMessage()) {
            ProtoView v(mModel.getMessage());
            v.exec();
        }
    } catch(const ProtoTreeError& e) {
        onProcessProtoError(e);
    }
}

void MainWindow::onProcessProtoError(const ProtoTreeError & e)
{
    QMessageBox::critical(this, e.what(), e.details());
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if( watched == ui->tvProtoTree &&
        event->type() == QEvent::KeyPress &&
        static_cast<QKeyEvent*>(event)->key() == Qt::Key_Delete) {
        mModel.onClearField(ui->tvProtoTree->currentIndex());
    }
    return QMainWindow::eventFilter(watched, event);
}
