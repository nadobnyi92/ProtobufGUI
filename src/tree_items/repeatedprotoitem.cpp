#include "repeatedprotoitem.h"

RepeatedProtoItem::RepeatedProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : ProtoTreeItem(field, parentItem) {}

void RepeatedProtoItem::addItem()
{
    createNode(field(), false);
    expand();
}

QBrush RepeatedProtoItem::color() const
{
    return QBrush(QColor(255, 0, 0, 90));
}

QItemDelegate *RepeatedProtoItem::getDelegate() const
{
    return nullptr;
}

void RepeatedProtoItem::setFieldValue(google::protobuf::Message *message)
{
    for(auto& child: childItems())
    {
        child->addFieldValue(message, field());
    }
}


void RepeatedProtoItem::initFieldValue(const google::protobuf::Message * message)
{
    clearChildren();
    for(int i = 0, size = message->GetReflection()->FieldSize(*message, field()); i < size; ++i)
    {
        createNode(field(), false)->initRepeatedFieldValue(message, i);
    }
}

void RepeatedProtoItem::clearValue()
{
    clearChildren();
}
