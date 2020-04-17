#include "rootprotoitem.h"
#include "../prototreeerror.h"

#include <fstream>

RootProtoItem::RootProtoItem(const proto::Descriptor *protoclass)
    : ProtoTreeItem(protoclass, nullptr), MessageProtoItem(protoclass, nullptr) {}

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
    for(auto& child: childItems()) {
        if( child->label() == proto::FieldDescriptor::LABEL_REPEATED ||
            message->GetReflection()->HasField(*message, child->field())) {
            child->initFieldValue(message);
        } else {
            child->clearValue();
        }
    }
}
