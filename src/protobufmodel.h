#ifndef PROTOBUFMODEL_H
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
    ProtobufModel(QObject * parent = nullptr) : QAbstractItemModel(parent) {}
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

public slots:
    void setProtoClass(const proto::Descriptor * protoclass);
    void onExpand(const QModelIndex& index);
    void onAddItem(const QModelIndex& index);
    void onRemoveItem(const QModelIndex& index);
    void onReplaceType(const QModelIndex& index, const proto::Descriptor * desc);
    void onClearData();

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

private:
    int itemIndex(ProtoTreeItem* item) const;
    ProtoTreeItem* toItem(const QModelIndex& idx) const;

    QIcon icon(ProtoTreeItem* item) const;
    std::string mDataFilePath;


private:
    std::unique_ptr<RootProtoItem> mRootItem;
};


#endif // PROTOBUFMODEL_H
