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
        for(auto& s: sMessage)
            printf("0x%02hhx ", s);
        std::cout << std::endl;
    }
    return sMessage;
}

void RootProtoItem::initMessage(const std::string& fp)
{
    std::ifstream is(fp.c_str());
    std::string str((std::istreambuf_iterator<char>(is)),
                     std::istreambuf_iterator<char>());

    for(auto& s: str)
        printf("0x%02hhx ", s);
    std::cout << std::endl;

    proto::Message * m = mFactory.GetPrototype(descriptor())->New();
//    if(!m->ParseFromIstream(&is))
//    {
//        throw ProtoTreeError(m);
//    }
//    std::cout << "MESSGE:\n" << m->DebugString() << std::endl;

    if(m->ParseFromString(str))
    {
        std::cout << "MESSGE:\n" << m->DebugString() << std::endl;
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
