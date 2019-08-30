#ifndef PROTOTREEITEM_H
#define PROTOTREEITEM_H

#include <QVector>
#include <QVariant>

#include <vector>
#include <bits/unique_ptr.h>

#include <google/protobuf/dynamic_message.h>

namespace proto = google::protobuf;

class ProtoTreeItem
{
public:

    struct ProtoData
    {
        ProtoData(const google::protobuf::Descriptor *protoclass);
        ProtoData(const google::protobuf::FieldDescriptor * field);
        ProtoData(const std::string& n) : name(n) {}

        std::string name;
        proto::FieldDescriptor::Type type;
        proto::FieldDescriptor::Label label;
        std::string value;
    };

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
