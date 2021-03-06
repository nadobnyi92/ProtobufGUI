#include "prototreeitem.h"

#include<algorithm>

#include <QIcon>
#include <QPainter>

#include "numericprotoitem.h"
#include "floatprotoitem.h"
#include "stringprotoitem.h"
#include "boolprotoitem.h"
#include "enumprotoitem.h"
#include "repeatedprotoitem.h"
#include "messageprotoitem.h"
#include "bytestprotoitem.h"

ProtoTreeItem::ProtoTreeItem(const google::protobuf::Descriptor *pclass, ProtoTreeItem *parentItem)
    : QObject(nullptr)
    , mField(nullptr)
    , mName(pclass->name().c_str())
    , mDesc(pclass)
    , mParentItem(parentItem) {}

ProtoTreeItem::ProtoTreeItem(const google::protobuf::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : QObject(nullptr)
    , mField(field)
    , mName(field->name().c_str())
    , mDesc(field->message_type())
    , mParentItem(parentItem) {}

void ProtoTreeItem::expand()
{
    expandChildren();
    for(auto& child: mChildItems)
        child->expandChildren();
}

void ProtoTreeItem::expandChildren()
{
    if( !isRepeated() && isMessageType() && !isExpanded() ) {
        for(int i = 0; i < mDesc->field_count(); ++i) {
            if(mDesc->field(i)->label() == proto::FieldDescriptor::LABEL_REPEATED)
                createRepeatedNode(mDesc->field(i));
            else
                createNode(mDesc->field(i));
        }
    }
}

void ProtoTreeItem::clearChildren()
{
    mChildItems.clear();
}

bool ProtoTreeItem::isRepeated() const
{
    return mField != nullptr && mField->label() == proto::FieldDescriptor::LABEL_REPEATED;
}

bool ProtoTreeItem::isRequired() const
{
    return mField == nullptr || mField->label() == proto::FieldDescriptor::LABEL_REQUIRED;
}

bool ProtoTreeItem::isExpanded() const
{
    return mChildItems.size() > 0;
}

bool ProtoTreeItem::isMessageType() const
{
    return mDesc != nullptr;
}

const google::protobuf::Descriptor *ProtoTreeItem::descriptor() const
{
    return mDesc;
}

const std::vector<std::unique_ptr<ProtoTreeItem> > &ProtoTreeItem::children() const
{
    return mChildItems;
}

const google::protobuf::FieldDescriptor *ProtoTreeItem::field() const
{
    return mField;
}

std::unique_ptr<ProtoTreeItem> &ProtoTreeItem::createRepeatedNode(const google::protobuf::FieldDescriptor *field)
{
    mChildItems.push_back(std::unique_ptr<RepeatedProtoItem>(new RepeatedProtoItem(field, this)));
    return mChildItems.back();
}

std::unique_ptr<ProtoTreeItem>& ProtoTreeItem::createNode(const google::protobuf::FieldDescriptor *field)
{
    switch(field->type())
    {
        case proto::FieldDescriptor::TYPE_DOUBLE:
        case proto::FieldDescriptor::TYPE_FLOAT:
            mChildItems.push_back(std::unique_ptr<FloatProtoItem>(new FloatProtoItem(field, this)));
            break;
        case proto::FieldDescriptor::TYPE_INT64:
        case proto::FieldDescriptor::TYPE_UINT64:
        case proto::FieldDescriptor::TYPE_INT32:
        case proto::FieldDescriptor::TYPE_FIXED64:
        case proto::FieldDescriptor::TYPE_FIXED32:
        case proto::FieldDescriptor::TYPE_SFIXED32:
        case proto::FieldDescriptor::TYPE_SFIXED64:
        case proto::FieldDescriptor::TYPE_SINT32:
        case proto::FieldDescriptor::TYPE_SINT64:
        case proto::FieldDescriptor::TYPE_UINT32:
            mChildItems.push_back(std::unique_ptr<NumericProtoItem>(new NumericProtoItem(field, this)));
            break;
        case proto::FieldDescriptor::TYPE_BOOL:
            mChildItems.push_back(std::unique_ptr<BoolProtoItem>(new BoolProtoItem(field, this)));
            break;
        case proto::FieldDescriptor::TYPE_BYTES:
            mChildItems.push_back(std::unique_ptr<BytesProtoItem>(new BytesProtoItem(field, this)));
        break;
        case proto::FieldDescriptor::TYPE_STRING:
            mChildItems.push_back(std::unique_ptr<StringProtoItem>(new StringProtoItem(field, this)));
            break;
        case proto::FieldDescriptor::TYPE_GROUP:
        case proto::FieldDescriptor::TYPE_MESSAGE:
            mChildItems.push_back(std::unique_ptr<MessageProtoItem>(new MessageProtoItem(field, this)));
            break;
        case proto::FieldDescriptor::TYPE_ENUM:
            mChildItems.push_back(std::unique_ptr<EnumProtoItem>(new EnumProtoItem(field, this)));
            break;
        default: break; //TODO add exception
    }
    return mChildItems.back();
}

void ProtoTreeItem::setValue(const QVariant &data)
{
    mValue = data;
}

void ProtoTreeItem::setDesc(const google::protobuf::Descriptor *desc)
{
    if(desc == nullptr)
        return;

    mChildItems.clear();
    mDesc = desc;
    expand();
}

void ProtoTreeItem::removeRow(int row)
{
    mChildItems.erase(mChildItems.begin() + row);
}

void ProtoTreeItem::clearValue()
{
    mValue = QVariant();
}

QString ProtoTreeItem::name() const
{
    return mName;
}

QVariant ProtoTreeItem::value() const
{
    return mValue;
}

QString ProtoTreeItem::typeName() const
{
    return mField ? mField->type_name() : "Message";
}

ProtoTreeItem::ItemState ProtoTreeItem::state() const
{
    if(mValue.isValid())
        return STATE_FILL;
    return isRequired() ? STATE_EMPTY : STATE_OPTIONAL;
}

ProtoTreeItem *ProtoTreeItem::parentItem()
{
    return mParentItem;
}
