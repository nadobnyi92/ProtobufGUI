#ifndef BYTESTPROTOITEM_H
#define BYTESTPROTOITEM_H

#include "prototreeitem.h"
#include "stringprotoitem.h"
#include "messageprotoitem.h"

#include <QItemDelegate>

class BytesProtoItem : public StringProtoItem, public MessageProtoItem
{
public:
    BytesProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem = nullptr);

    void setDesc(const proto::Descriptor * desc);

    // ProtoTreeItem interface
public:
    QItemDelegate *getDelegate() const override;
    void clearValue() override;

protected:
    void fillFieldValue(google::protobuf::Message *) override;
    void addFieldValue(google::protobuf::Message *, const google::protobuf::FieldDescriptor *) override;
    void initFieldValue(const google::protobuf::Message *) override;
    void initRepeatedFieldValue(const google::protobuf::Message *, int idx) override;

private:
    std::string getSerializedMessage();

private:
    std::string sourceData;
};

#endif // BYTESTPROTOITEM_H
