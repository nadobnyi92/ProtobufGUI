#ifndef ROOTPROTOITEM_H
#define ROOTPROTOITEM_H

#include "messageprotoitem.h"

class RootProtoItem : public MessageProtoItem
{
public:
    RootProtoItem(const proto::Descriptor *protoclass, ProtoTreeItem *parentItem = nullptr);
    std::string getStringMessage();
};

#endif // ROOTPROTOITEM_H
