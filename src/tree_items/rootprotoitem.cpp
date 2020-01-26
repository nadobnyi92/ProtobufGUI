#include "rootprotoitem.h"
#include "../prototreeerror.h"

#include <fstream>

RootProtoItem::RootProtoItem(const proto::Descriptor *protoclass)
    : ProtoTreeItem(protoclass, nullptr), MessageProtoItem(protoclass, nullptr) {}

std::string RootProtoItem::getStringMessage()
{
    std::string sMessage;
    proto::Message * m = getMessage();
    if(m != nullptr)
    {
        std::cout << "MESSAGE:\n" << m->Utf8DebugString() <<std::endl;
        sMessage = m->SerializeAsString();
    }
    return sMessage;
}

void RootProtoItem::initMessage(const std::string& fp)
{
    std::ifstream is;
    is.open(fp.c_str(), std::ios::binary);

    proto::Message * m = mFactory.GetPrototype(descriptor())->New();
    if(!m->ParseFromIstream(&is))
    {
        throw ProtoTreeError(m);
    }
    initFieldValue(m);
}


void RootProtoItem::initFieldValue(const google::protobuf::Message * message)
{
    for(auto& child: childItems())
    {
        if( child->label() == proto::FieldDescriptor::LABEL_REPEATED ||
            message->GetReflection()->HasField(*message, child->field()))
        {
            child->initFieldValue(message);
        }
        else
        {
            child->clearValue();
        }
    }
}
