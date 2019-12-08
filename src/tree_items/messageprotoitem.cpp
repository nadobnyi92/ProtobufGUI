#include "messageprotoitem.h"

MessageProtoItem::MessageProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : ProtoTreeItem(field, parentItem) {}

MessageProtoItem::MessageProtoItem(const google::protobuf::Descriptor *protoclass, ProtoTreeItem *parentItem)
    : ProtoTreeItem(protoclass, parentItem) {}

MessageProtoItem::~MessageProtoItem() {}

QBrush MessageProtoItem::color() const
{
    return QBrush(QColor(255, 0, 0, 90));
}

QItemDelegate *MessageProtoItem::getDelegate() const
{
    return nullptr;
}

void MessageProtoItem::setFieldValue(google::protobuf::Message *message)
{
    message->GetReflection()->SetAllocatedMessage(message, getMessage(), field());
}


proto::Message * MessageProtoItem::getMessage()
{
    if(descriptor() == nullptr)
        return nullptr;
    proto::Message * m = mFactory.GetPrototype(descriptor())->New();

    for(auto& child: childItems())
    {
        child->setFieldValue(m);
    }
    return m;
}


void MessageProtoItem::addFieldValue(google::protobuf::Message * message, const google::protobuf::FieldDescriptor * desk)
{
    message->GetReflection()->AddAllocatedMessage(message, desk, getMessage());
}


void MessageProtoItem::initFieldValue(const google::protobuf::Message * message)
{
    const proto::Message& m = message->GetReflection()->GetMessage(*message, field());
    expandChildren();
    for(auto& child: childItems())
    {
        child->initFieldValue(&m);
    }
}
