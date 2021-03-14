#include "rootprotoitem.h"
#include "../prototreeerror.h"

#include <fstream>

RootProtoItem::RootProtoItem(ProtobufModel& model, const proto::Descriptor *protoclass)
    : ProtoTreeItem(model, protoclass, nullptr), MessageProtoItem(model, protoclass, nullptr) {}

std::string RootProtoItem::getStringMessage()
{
    std::string sMessage;
    proto::Message * m = getMessage();
    if(m != nullptr) {
        try {
            sMessage = m->SerializeAsString();
        } catch (google::protobuf::FatalException& e) {
            throw ProtoTreeError("Failed serialize message", e.message());
        }

        printHex(sMessage);
    }
    return sMessage;
}

google::protobuf::Message *RootProtoItem::getMessage()
{
    return MessageProtoItem::getMessage();
}

void RootProtoItem::initMessage(const std::string& fp)
{
    std::ifstream is(fp.c_str());
    std::string str((std::istreambuf_iterator<char>(is)),
                     std::istreambuf_iterator<char>());
    proto::Message * m = mFactory.GetPrototype(descriptor())->New();
    if(m->ParseFromString(str))
        printHex(m);
    initFieldValue(m);
}


void RootProtoItem::initFieldValue(const google::protobuf::Message * message)
{
    for(auto& child: children()) {
        if( child->field()->label() == proto::FieldDescriptor::LABEL_REPEATED ||
            message->GetReflection()->HasField(*message, child->field())) {
            child->initFieldValue(message);
        } else {
            child->clearValue();
        }
    }
}
