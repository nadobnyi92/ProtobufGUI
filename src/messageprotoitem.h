#ifndef MESSAGEPROTOITEM_H
#define MESSAGEPROTOITEM_H

#include "prototreeitem.h"
class MessageProtoItem : public ProtoTreeItem
{
public:
    MessageProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem = nullptr);
    MessageProtoItem(const proto::Descriptor *protoclass, ProtoTreeItem *parentItem = nullptr);

    ~MessageProtoItem() override;

    // ProtoTreeItem interface
public:
    QBrush color() const override;
    QItemDelegate *getDelegate() const override;

protected:
    void setFieldValue(google::protobuf::Message *message) override;
    proto::Message *getMessage();

private:
    proto::DynamicMessageFactory mFactory;

};

#endif // MESSAGEPROTOITEM_H
