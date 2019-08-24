#include "prototreeitem.h"

#include<algorithm>

ProtoTreeItem::ProtoTreeItem(const QVector<QVariant> &data, ProtoTreeItem *parentItem)
    : mItemData(data), mParentItem(parentItem) {}

ProtoTreeItem::~ProtoTreeItem()
{
    qDeleteAll(mChildItems);
}

void ProtoTreeItem::appendChild(ProtoTreeItem *child)
{
    mChildItems.push_back(child);
}

ProtoTreeItem* ProtoTreeItem::child(int row)
{
    return row >= 0 && row < mChildItems.size() ? mChildItems.at(row) : nullptr;
}

int ProtoTreeItem::childCount() const
{
    return mChildItems.size();
}

int ProtoTreeItem::columnCount() const
{
    return mItemData.size();
}

QVariant ProtoTreeItem::data(int column) const
{
    return column >= 0 && column < mItemData.size() ? mItemData.at(column) : QVariant();
}

int ProtoTreeItem::row() const
{
    if(mParentItem == nullptr)
        return 0;
    return mParentItem->mChildItems.indexOf(const_cast<ProtoTreeItem*>(this));
}

ProtoTreeItem *ProtoTreeItem::parentItem()
{
    return mParentItem;
}



