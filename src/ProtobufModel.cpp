#include "ProtobufModel.h"
#include "prototreeitem.h"

void ProtobufModel::setProtoClass(const google::protobuf::Descriptor *protoclass)
{
    mProtoClass = protoclass;
    if(mProtoClass != nullptr)
    {
        emit beginResetModel();
        mRootItem = std::make_unique<ProtoTreeItem>(protoclass);
        mRootItem->expand();
        emit endResetModel();
    }
}

void ProtobufModel::onDoubleClick(const QModelIndex &index)
{
    if (!mRootItem || !index.isValid())
        return;

    ProtoTreeItem *item = static_cast<ProtoTreeItem*>(index.internalPointer());

    beginInsertRows(index, 0, item->fieldCount());
    item->expand();
    endInsertRows();
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
    return ProtoTreeItem::columnCount();
}

QVariant ProtobufModel::data(const QModelIndex &index, int role) const
{
    if (!mRootItem || !index.isValid())
        return QVariant();

    ProtoTreeItem *item = static_cast<ProtoTreeItem*>(index.internalPointer());

    switch (role)
    {
        case Qt::DisplayRole:
            return item->data(index.column());
        case Qt::BackgroundRole:
            return index.column() > 0 ? item->color() : QVariant();
        case Qt::DecorationRole:
            return index.column() == 0 ? item->icon() : QVariant();

    }
    return QVariant();
}

QVariant ProtobufModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case 0:
                return "";
            case 1:
                return "Поле";
            case 2:
                return "Тип данных";
            case 3:
                return "Значение";
        }
    }
    return QVariant();
}
