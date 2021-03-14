#include "messageprotoitem.h"

MessageProtoItem::MessageProtoItem(ProtobufModel& model, const proto::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : ProtoTreeItem(model, field, parentItem) {}

MessageProtoItem::MessageProtoItem(ProtobufModel& model, const google::protobuf::Descriptor *protoclass, ProtoTreeItem *parentItem)
    : ProtoTreeItem(model, protoclass, parentItem) {}

MessageProtoItem::~MessageProtoItem() {}

QItemDelegate *MessageProtoItem::getDelegate() const
{
    return nullptr;
}

void MessageProtoItem::fillFieldValue(google::protobuf::Message *message)
{
    message->GetReflection()->SetAllocatedMessage(message, getMessage(), field());
}

proto::Message * MessageProtoItem::getMessage()
{
    if(descriptor() == nullptr)
        return nullptr;
    proto::Message * m = mFactory.GetPrototype(descriptor())->New();

    for(auto& child: children())
        child->fillFieldValue(m);
    return m;
}

void MessageProtoItem::printHex(const google::protobuf::Message * m) const
{
    printf("Message data:%s\n", m->DebugString().c_str());
    printHex(m->SerializeAsString());
}

void MessageProtoItem::printHex(const std::string & msg, size_t line_width) const
{
    int n = 0;
    printf("Message:\n%s\nHEX:\n", msg.c_str());
    for(auto& s: msg) {
        if((++n) % line_width == 0)
            printf("\n");
        printf("0x%02hhx ", s);
    }
    printf("\n");
    fflush(stdout);
}

void MessageProtoItem::initChildValues(const google::protobuf::Message &m)
{
    for(auto& child: children())
        child->initFieldValue(&m);
}

void MessageProtoItem::addFieldValue(google::protobuf::Message * message, const google::protobuf::FieldDescriptor * desk)
{
    message->GetReflection()->AddAllocatedMessage(message, desk, getMessage());
}

void MessageProtoItem::initFieldValue(const google::protobuf::Message * message)
{
    expandChildren();
    const proto::Message& m = message->GetReflection()->GetMessage(*message, field());
    for(auto& child: children()) {
        if( child->field()->label() == proto::FieldDescriptor::LABEL_REPEATED ||
            message->GetReflection()->HasField(m, child->field())) {
            child->initFieldValue(&m);
        } else {
            child->clearValue();
        }
    }
}

void MessageProtoItem::initRepeatedFieldValue(const google::protobuf::Message * message, int idx)
{
    const proto::Message& m = message->GetReflection()->GetRepeatedMessage(*message, field(), idx);
    for(int i = 0; i < descriptor()->field_count(); ++i) {
        createNode(descriptor()->field(i))->initFieldValue(&m);
    }
}

void MessageProtoItem::clearValue()
{
    for(auto& child: children()) {
        child->clearValue();
    }
}

ProtoTreeItem::ItemState MessageProtoItem::state() const
{
    auto it = std::find_if( children().begin(), children().end(),
                            [](const ProtoTreeItem* child) { return child->state() == STATE_EMPTY; });
    if(it == children().end())
        return STATE_FILL ;
    return isRequired() ? STATE_EMPTY : STATE_OPTIONAL;
}
