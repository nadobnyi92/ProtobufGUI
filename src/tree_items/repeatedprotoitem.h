#ifndef REPEATEDPROTOITEM_H
#define REPEATEDPROTOITEM_H

#include "prototreeitem.h"

class RepeatedProtoItem : public ProtoTreeItem
{
    Q_OBJECT

public:
    RepeatedProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem = nullptr);

public slots:
    void addItem();

    // ProtoTreeItem interface
public:
    QBrush color() const override;
    QItemDelegate *getDelegate() const override;

protected:
    void fillFieldValue(google::protobuf::Message *message) override;
    void addFieldValue(google::protobuf::Message *, const google::protobuf::FieldDescriptor *) override {}
    void initFieldValue(const google::protobuf::Message *) override;
    void initRepeatedFieldValue(const google::protobuf::Message *, int) override {}

    // ProtoTreeItem interface
public:
    void clearValue() override;
};

#endif // REPEATEDPROTOITEM_H
