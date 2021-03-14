#ifndef REPEATEDPROTOITEM_H
#define REPEATEDPROTOITEM_H

#include "prototreeitem.h"

class RepeatedProtoItem : public ProtoTreeItem
{
    Q_OBJECT

public:
    RepeatedProtoItem(ProtobufModel& model, const proto::FieldDescriptor * field, ProtoTreeItem *parentItem = nullptr);

    void clearValue() override;
    QItemDelegate *getDelegate() const override;
    QString name() const override;

protected:
    void fillFieldValue(google::protobuf::Message *message) override;
    void addFieldValue(google::protobuf::Message *, const google::protobuf::FieldDescriptor *) override {}
    void initFieldValue(const google::protobuf::Message *) override;
    void initRepeatedFieldValue(const google::protobuf::Message *, int) override {}

private:
    void init();
};

#endif // REPEATEDPROTOITEM_H
