#ifndef PROTOBUFMODEL_H
#define PROTOBUFMODEL_H

#include <QAbstractItemModel>

#include <google/protobuf/dynamic_message.h>

#include "prototreeitem.h"

class ProtoTreeItem;

namespace proto = google::protobuf;

class ProtobufModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ProtobufModel(QObject * parent = nullptr) : QAbstractItemModel(parent) {}
    ~ProtobufModel() override {}

public slots:
    void setProtoClass(const proto::Descriptor * protoclass);
    void onDoubleClick(const QModelIndex& index);

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    proto::Descriptor const * mProtoClass;
    std::unique_ptr<ProtoTreeItem> mRootItem;
};

#endif // PROTOBUFMODEL_H
