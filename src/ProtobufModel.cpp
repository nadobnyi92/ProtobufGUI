#include "ProtobufModel.h"
#include "prototreeitem.h"

ProtobufModel::ProtobufModel(const google::protobuf::Descriptor *pclass)
{
    mRoleMapping[ProtoNameRole] = "name";
    mRoleMapping[ProtoTypeRole] = "type";
    mRoleMapping[ProtoRoleRole] = "role";
    mRoleMapping[ProtoValueRole] = "value";

    setProtoClass(pclass);
}

ProtobufModel::~ProtobufModel() {}

void ProtobufModel::setProtoClass(const google::protobuf::Descriptor *protoclass)
{
    emit beginResetModel();
    mProtoClass = protoclass;
    if(mProtoClass != nullptr)
    {
        mRootItem = std::make_unique<ProtoTreeItem>(ProtoTreeItem::ProtoData(protoclass));
        for(int i = 0; i < protoclass->field_count(); ++i)
        {
            mRootItem->appendChild(ProtoTreeItem::ProtoData(protoclass->field(i)));
        }
    }
    emit endResetModel();

}

QModelIndex ProtobufModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!mRootItem || !hasIndex(row,column,parent))
        return QModelIndex();

    ProtoTreeItem *parentItem;

    if (!parent.isValid())
            parentItem = mRootItem.get();
        else
            parentItem = static_cast<ProtoTreeItem*>(parent.internalPointer());

    ProtoTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex ProtobufModel::parent(const QModelIndex &index) const
{    
    if (!mRootItem || !index.isValid())
        return QModelIndex();

    ProtoTreeItem *childItem = static_cast<ProtoTreeItem*>(index.internalPointer());
    ProtoTreeItem *parentItem = childItem->parentItem();

    if (parentItem == mRootItem.get())
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ProtobufModel::rowCount(const QModelIndex &parent) const
{
    ProtoTreeItem *parentItem;

    if (!mRootItem || parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = mRootItem.get();
    else
        parentItem = static_cast<ProtoTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int ProtobufModel::columnCount(const QModelIndex &parent) const
{
    return mRoleMapping.size();
}

QVariant ProtobufModel::data(const QModelIndex &index, int role) const
{
    if (!mRootItem || !index.isValid())
        return QVariant();

    ProtoTreeItem *item = static_cast<ProtoTreeItem*>(index.internalPointer());

    switch (role)
    {
        case ProtoNameRole:
            return item->data(0);
        case ProtoTypeRole:
            return item->data(1);
        case ProtoRoleRole:
            return item->data(2);
        case ProtoValueRole:
            return item->data(3);
    }
    return QVariant();
}

QHash<int, QByteArray> ProtobufModel::roleNames() const
{
    return mRoleMapping;
}
