#include "rootprotoitem.h"

#include <fstream>

RootProtoItem::RootProtoItem(const proto::Descriptor *protoclass, ProtoTreeItem *parentItem)
    : MessageProtoItem(protoclass, parentItem) {}

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

bool RootProtoItem::initMessage(const std::string& fp)
{
    std::ifstream is;
    is.open(fp.c_str(), std::ios::binary);

    proto::Message * m = mFactory.GetPrototype(descriptor())->New();
    if(!m->ParseFromIstream(&is))
        return false;
    initFieldValue(m);
    std::cout << m->DebugString() << std::endl;
    return true;
}


void RootProtoItem::initFieldValue(const google::protobuf::Message * message)
{
    expandChildren();
    for(auto& child: childItems())
    {
        child->initFieldValue(message);
    }
}