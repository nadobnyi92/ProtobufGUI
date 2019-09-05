#ifndef PROTOTREEITEM_H
#define PROTOTREEITEM_H

#include <QVector>
#include <QVariant>

#include <vector>
#include <bits/unique_ptr.h>

#include <google/protobuf/dynamic_message.h>

#include "protodata.h"

namespace proto = google::protobuf;

class ProtoTreeItem
{
public:
    ProtoTreeItem(const ProtoData &data, ProtoTreeItem *parentItem = nullptr);

    size_t childCount() const;
    size_t columnCount() const;
    QVariant data(int column) const;
    size_t row() const;
    ProtoTreeItem *parentItem();

    void appendChild(const ProtoData& data);
    ProtoTreeItem *child(size_t row);

private:
    std::vector< std::unique_ptr<ProtoTreeItem> > mChildItems;
    ProtoData mItemData;
    ProtoTreeItem *mParentItem;
};

#endif // PROTOTREEITEM_H
