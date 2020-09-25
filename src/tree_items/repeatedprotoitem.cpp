#include "repeatedprotoitem.h"

RepeatedProtoItem::RepeatedProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : ProtoTreeItem(field, parentItem) {}

void RepeatedProtoItem::addItem()
{
    createNode(field());
    expand();
}

QItemDelegate *RepeatedProtoItem::getDelegate() const
{
    return nullptr;
}

void RepeatedProtoItem::fillFieldValue(google::protobuf::Message *message)
{
    for(auto& child: children())
        child->addFieldValue(message, field());
}


void RepeatedProtoItem::initFieldValue(const google::protobuf::Message * message)
{
    clearChildren();
    for(int i = 0, size = message->GetReflection()->FieldSize(*message, field()); i < size; ++i)
        createNode(field())->initRepeatedFieldValue(message, i);
}

void RepeatedProtoItem::clearValue()
{
    clearChildren();
}
