#include "prototreeitem.h"

#include<algorithm>

ProtoTreeItem::ProtoData::ProtoData(const google::protobuf::Descriptor *protoclass)
{
    name = protoclass->name();
    label = proto::FieldDescriptor::LABEL_REQUIRED;
    type = proto::FieldDescriptor::TYPE_MESSAGE;
}

ProtoTreeItem::ProtoData::ProtoData(const google::protobuf::FieldDescriptor * field)
{
    name = field->name();
    label = field->label();
    type = field->type();
}

ProtoTreeItem::ProtoTreeItem(const ProtoData &data, ProtoTreeItem *parentItem)
    : mItemData(data), mParentItem(parentItem) {}

void ProtoTreeItem::appendChild(const ProtoData& data)
{
    mChildItems.push_back(std::make_unique<ProtoTreeItem>(data, this));
}

ProtoTreeItem* ProtoTreeItem::child(size_t row)
{
    return row < mChildItems.size() ? mChildItems.at(row).get() : nullptr;
}

size_t ProtoTreeItem::childCount() const
{
    return mChildItems.size();
}

size_t ProtoTreeItem::columnCount() const
{
    return 4;
}

QVariant ProtoTreeItem::data(int column) const
{
    switch(column)
    {
        case 0:
            return mItemData.name.c_str();
        case 1:
            return mItemData.type;
        case 2:
            return mItemData.label;
        case 3:
            return mItemData.value.c_str();
    }
    return QVariant();
}

size_t ProtoTreeItem::row() const
{
    if(mParentItem == nullptr)
        return 0;

    auto it = std::find_if(mParentItem->mChildItems.begin(), mParentItem->mChildItems.end(),
                           [this](auto&& val) { return val.get() == this; });
    return std::distance(mParentItem->mChildItems.begin(), it);
}

ProtoTreeItem *ProtoTreeItem::parentItem()
{
    return mParentItem;
}



