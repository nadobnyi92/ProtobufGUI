#include "rootprotoitem.h"

RootProtoItem::RootProtoItem(const proto::Descriptor *protoclass, ProtoTreeItem *parentItem)
    : MessageProtoItem(protoclass, parentItem)
{

}

std::string RootProtoItem::getStringMessage()
{
    std::string sMessage;
    proto::Message * m = getMessage();
    if(m != nullptr)
    {
        std::cout << "MESSAGE:\n" << m->Utf8DebugString() <<std::endl;
        //sMessage = m->SerializeAsString();
        //delete m;
    }
    return sMessage;
}
