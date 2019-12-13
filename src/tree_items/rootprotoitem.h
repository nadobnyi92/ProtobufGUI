#ifndef ROOTPROTOITEM_H
#define ROOTPROTOITEM_H

#include "messageprotoitem.h"

class RootProtoItem : public MessageProtoItem
{
public:
    RootProtoItem(const proto::Descriptor *protoclass, ProtoTreeItem *parentItem = nullptr);
    std::string getStringMessage();
    bool initMessage(const std::string& fp);

    // ProtoTreeItem interface
protected:
    void initFieldValue(const google::protobuf::Message *) override;
};

#endif // ROOTPROTOITEM_H
