#include <iostream>

#include <QFileDialog>
#include <QMessageBox>
#include <QMap>
#include <QKeyEvent>
#include <QDebug>

#include "mainwindow.h"
#include "protomanager.h"
#include "fielddelegate.h"

#include "prototypedialog.h"

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

    connect(ui->tvProtoTree, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(onPrepareMenu(const QPoint&)));
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

void MainWindow::onPrepareMenu(const QPoint &p)
{
    QModelIndex idx = ui->tvProtoTree->indexAt(p);
    ProtoTreeItem *item = static_cast<ProtoTreeItem*>(idx.internalPointer());

    QMenu menu;

    if(item->label() == proto::FieldDescriptor::LABEL_REPEATED)
    {
        QAction * actAddItem = new QAction("Добавить элемент");
        actAddItem->setData(idx);
        connect(actAddItem, SIGNAL(triggered()), SLOT(onAddItem()));
        menu.addAction(actAddItem);
    }
    else
    {
        if(item->type() == proto::FieldDescriptor::TYPE_BYTES)
        {
            QAction * actTransform = new QAction("Преобразовать тип");
            actTransform->setData(idx);
            connect(actTransform, SIGNAL(triggered()), SLOT(onReplaceType()));
            menu.addAction(actTransform);
        }
        if(item->parentItem()->label() == proto::FieldDescriptor::LABEL_REPEATED)
        {
            QAction * actRemove = new QAction("Удалить элемент");
            actRemove->setData(idx);
            connect(actRemove, SIGNAL(triggered()), SLOT(onRemoveItem()));
            menu.addAction(actRemove);
        }
    }

    if(menu.actions().size() > 0)
    {
        menu.exec( ui->tvProtoTree->mapToGlobal(p) );
    }
}

void MainWindow::onAddItem()
{
    QAction * act = static_cast<QAction*>(sender());
    QModelIndex idx = qvariant_cast<QModelIndex>(act->data());
    mModel.onAddItem(idx);
    ui->tvProtoTree->expand(idx);
}

void MainWindow::onRemoveItem()
{
    QAction * act = static_cast<QAction*>(sender());
    QModelIndex idx = qvariant_cast<QModelIndex>(act->data());
    mModel.onRemoveItem(idx);
}

void MainWindow::onReplaceType()
{
    ProtoTypeDialog dlg(mProtoManager, this);
    if(dlg.exec() == QDialog::Accepted)
    {
        QAction * act = static_cast<QAction*>(sender());
        QModelIndex idx = qvariant_cast<QModelIndex>(act->data());
        mModel.onReplaceType(idx,
            mProtoManager.getClassDescriptor(dlg.pPackage(), dlg.pClass()));
        ui->tvProtoTree->expand(idx);
    }
}

void MainWindow::onSaveProtoData()
{
    QString message = mModel.getMessage();
    if(!message.isEmpty())
    {
        QFile f(QFileDialog::getSaveFileName());
        if(f.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&f);
            stream << message;
        }
    }
}

void MainWindow::onLoadProtoData()
{
    try
    {
        mModel.loadProtoData();
    }
    catch (const ProtobufModel::ProtoInitException& e)
    {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if( watched == ui->tvProtoTree &&
        event->type() == QEvent::KeyPress )
    {
        QKeyEvent * kEvent = static_cast<QKeyEvent*>(event);
        if(kEvent->key() == Qt::Key_Delete)
        {
            mModel.setData(ui->tvProtoTree->currentIndex().siblingAtColumn(ProtobufModel::COL_VALUE), QVariant(), Qt::EditRole);
        }
    }
    return QMainWindow::eventFilter(watched, event);
}
