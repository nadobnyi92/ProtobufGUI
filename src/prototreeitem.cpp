#include "prototreeitem.h"

#include<algorithm>

#include <QImage>
#include <QPainter>

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
            return mItemData.getName().c_str();
        case 1:
            return mItemData.getType().c_str();
        case 2:
            return mItemData.getLabel().c_str();
        case 3:
            return mItemData.getValue().c_str();
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
