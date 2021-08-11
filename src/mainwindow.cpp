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
    , mModel(mProtoManager)
{
    ui->setupUi(this);

    ui->tvProtoTree->setModel(&mModel);

    auto header = ui->tvProtoTree->header();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tvProtoTree->setItemDelegateForColumn(ProtobufModel::COL_VALUE, new FieldDelegate(this));
    ui->tvProtoTree->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->tvProtoTree->installEventFilter(this);

    connect(ui->tbLoad, &QToolButton::clicked,
            this, &MainWindow::onLoadClasses);
    connect(ui->tbReload, &QToolButton::clicked,
            &mProtoManager, &ProtoManager::reload);
    connect(ui->cbPackage, &QComboBox::currentTextChanged,
            &mProtoManager, &ProtoManager::setPackage);
    connect(ui->cbClass, &QComboBox::currentTextChanged,
            &mProtoManager, &ProtoManager::setClass);
    connect(&mProtoManager, &ProtoManager::onProtoChange,
            this, &MainWindow::onSetPackages);
    connect(&mProtoManager, &ProtoManager::onPackageChange,
            this, &MainWindow::onSetClasses);
    connect(&mProtoManager, &ProtoManager::onClassChange,
            &mModel, &ProtobufModel::setProtoClass);
    connect(ui->tvProtoTree, &QTreeView::expanded,
            &mModel, &ProtobufModel::onExpand);
    connect(ui->pbSaveData, &QPushButton::clicked,
            this, &MainWindow::onSaveProtoData);
    connect(ui->pbLoadData, &QPushButton::clicked,
            this, &MainWindow::onLoadProtoData);
    connect(ui->pbReject, &QPushButton::clicked,
            &mModel, &ProtobufModel::onClearData);
    connect(ui->pbView, &QPushButton::clicked,
            [this](){
            });

    connect(ui->tvProtoTree, &QTreeView::customContextMenuRequested,
            this, &MainWindow::onPrepareMenu);

    connect(&mModel, &ProtobufModel::processProtoError,
            this, &MainWindow::onProcessProtoError);
    connect(&mModel, &ProtobufModel::expandNode,
            ui->tvProtoTree, &QTreeView::expand);
}

void MainWindow::onLoadClasses(bool)
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
    auto actions = mModel.actions(ui->tvProtoTree->indexAt(p));
    if(actions.size() > 0) {
        QMenu menu;
        menu.addActions(actions);
        menu.exec( ui->tvProtoTree->mapToGlobal(p) );
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
