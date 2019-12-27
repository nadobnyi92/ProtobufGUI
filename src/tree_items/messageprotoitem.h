#ifndef MESSAGEPROTOITEM_H
#define MESSAGEPROTOITEM_H

#include "prototreeitem.h"
class MessageProtoItem : public virtual ProtoTreeItem
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
    void addFieldValue(google::protobuf::Message *, const google::protobuf::FieldDescriptor *) override;
    void initRepeatedFieldValue(const google::protobuf::Message *, int idx) override;
    void initFieldValue(const google::protobuf::Message *) override;

    proto::Message *getMessage();

protected:
    proto::DynamicMessageFactory mFactory;

private:
    void initChildValues(const proto::Message&);

    // ProtoTreeItem interface
public:
    void clearValue() override;
};

#endif // MESSAGEPROTOITEM_H
