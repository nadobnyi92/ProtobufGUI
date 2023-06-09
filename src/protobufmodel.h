﻿#ifndef PROTOBUFMODEL_H
#define PROTOBUFMODEL_H

#include <QAbstractItemModel>

#include <google/protobuf/dynamic_message.h>

#include "tree_items/rootprotoitem.h"

#include "prototreeerror.h"

class ProtoTreeItem;

namespace proto = google::protobuf;

class ProtobufModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ProtobufModel(const ProtoManager& manager, QObject * parent = nullptr)
        : QAbstractItemModel(parent)
        , mProtoManager(manager) {}
    ~ProtobufModel() override {}

    enum ColumnTypes
    {
        COL_NAME = 0,
        COL_TYPE,
        COL_VALUE,
        COL_COUNT
    };

    proto::Message* getMessage();
    std::string getSerializedMessage() const;
    void loadProtoData();

signals:
    void processProtoError(const ProtoTreeError&);
    void expandNode(const QModelIndex& index);

public slots:
    void setProtoClass(const proto::Descriptor * protoclass);
    void onExpand(const QModelIndex& index);
    void onClearData();
    void onClearField(const QModelIndex& index);

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QList<QAction*> actions(const QModelIndex& index);

private:
    int itemIndex(ProtoTreeItem* item) const;
    ProtoTreeItem* toItem(const QModelIndex& idx) const;
    QModelIndex fromItem(ProtoTreeItem* item, const QModelIndex& parent = QModelIndex()) const;

    QIcon icon(ProtoTreeItem* item) const;
    std::string mDataFilePath;


private:
    std::unique_ptr<RootProtoItem> mRootItem;
    const ProtoManager& mProtoManager;
};


#endif // PROTOBUFMODEL_H
