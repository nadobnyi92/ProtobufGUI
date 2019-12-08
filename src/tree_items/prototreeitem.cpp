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

ProtoTreeItem::ProtoTreeItem(const google::protobuf::Descriptor *pclass, ProtoTreeItem *parentItem)
    : QObject(nullptr)
    , mField(nullptr)
    , mName(pclass->name().c_str())
    , mTypeName("Message")
    , mType(proto::FieldDescriptor::TYPE_MESSAGE)
    , mLabel(proto::FieldDescriptor::LABEL_REQUIRED)
    , mDesc(pclass)
    , mParentItem(parentItem) {}

ProtoTreeItem::ProtoTreeItem(const google::protobuf::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : QObject(nullptr)
    , mField(field)
    , mName(field->name().c_str())
    , mTypeName(field->type_name())
    , mType(field->type())
    , mDesc(field->message_type())
    , mParentItem(parentItem)
{
    if(parentItem != nullptr && parentItem->label() == proto::FieldDescriptor::LABEL_REPEATED)
    {
        mLabel = proto::FieldDescriptor::LABEL_OPTIONAL;
    }
    else
    {
        mLabel = field->label();
    }

}

ProtoTreeItem* ProtoTreeItem::child(size_t row)
{
    return row < mChildItems.size() ? mChildItems.at(row).get() : nullptr;
}

void ProtoTreeItem::expand()
{
    expandChildren();
    for(auto& child: mChildItems)
    {
        child->expandChildren();
    }
}

void ProtoTreeItem::expandChildren()
{
    if( mLabel != proto::FieldDescriptor::LABEL_REPEATED &&
        mDesc != nullptr &&
        mChildItems.empty() )
    {
        for(int i = 0; i < mDesc->field_count(); ++i)
        {
            createNode(mDesc->field(i));
        }
     }
}

const google::protobuf::Descriptor *ProtoTreeItem::descriptor() const
{
    return mDesc;
}

const std::vector<std::unique_ptr<ProtoTreeItem> > &ProtoTreeItem::childItems() const
{
    return mChildItems;
}

const google::protobuf::FieldDescriptor *ProtoTreeItem::field() const
{
    return mField;
}

void ProtoTreeItem::createNode(const google::protobuf::FieldDescriptor *field, bool isRepeat)
{
    if(field->label() == proto::FieldDescriptor::LABEL_REPEATED && isRepeat)
    {
        mChildItems.push_back(std::make_unique<RepeatedProtoItem>(field, this));
    }
    else
    {
        switch(field->type())
        {
            case proto::FieldDescriptor::TYPE_DOUBLE:
            case proto::FieldDescriptor::TYPE_FLOAT:
                mChildItems.push_back(std::make_unique<FloatProtoItem>(field, this));
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
                mChildItems.push_back(std::make_unique<NumericProtoItem>(field, this));
                break;
            case proto::FieldDescriptor::TYPE_BOOL:
                mChildItems.push_back(std::make_unique<BoolProtoItem>(field, this));
                break;
            case proto::FieldDescriptor::TYPE_BYTES:
            case proto::FieldDescriptor::TYPE_STRING:
                mChildItems.push_back(std::make_unique<StringProtoItem>(field, this));
                break;
            case proto::FieldDescriptor::TYPE_GROUP:
            case proto::FieldDescriptor::TYPE_MESSAGE:
                mChildItems.push_back(std::make_unique<MessageProtoItem>(field, this));
                break;
            case proto::FieldDescriptor::TYPE_ENUM:
                mChildItems.push_back(std::make_unique<EnumProtoItem>(field, this));
                break;
        }
    }
}

void ProtoTreeItem::setValue(const QVariant &data)
{
    mValue = data;
}

void ProtoTreeItem::setDesc(const google::protobuf::Descriptor *desc)
{
    mChildItems.clear();
    mDesc = desc;
    expand();
}

void ProtoTreeItem::removeRow(int row)
{
    mChildItems.erase(mChildItems.begin() + row);
}

int ProtoTreeItem::rowCount() const
{
    return static_cast<int>(mChildItems.size());
}

QString ProtoTreeItem::name() const
{
    return label() == proto::FieldDescriptor::LABEL_REPEATED ?
        QString("%1 [%2]").arg(mName).arg(mChildItems.size()) : mName;
}

QVariant ProtoTreeItem::value() const
{
    return mValue;
}

google::protobuf::FieldDescriptor::Type ProtoTreeItem::type() const
{
    return mType;
}

QString ProtoTreeItem::typeName() const
{
    QString tName = mTypeName;
    if(mDesc != nullptr && mType != proto::FieldDescriptor::TYPE_MESSAGE)
        tName.append("(").append(mDesc->name().c_str()).append(")");
    return tName;
}

google::protobuf::FieldDescriptor::Label ProtoTreeItem::label() const
{
    return mLabel;
}

int ProtoTreeItem::row() const
{
    if(mParentItem == nullptr)
        return 0;
    auto it = std::find_if(mParentItem->mChildItems.begin(), mParentItem->mChildItems.end(),
        [this](auto&& val) { return val.get() == this; });
    return static_cast<int>(std::distance(mParentItem->mChildItems.begin(), it));
}

ProtoTreeItem *ProtoTreeItem::parentItem()
{
    return mParentItem;
}
