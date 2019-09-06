#include "prototreeitem.h"

#include<algorithm>

#include <QIcon>
#include <QPainter>

ProtoTreeItem::ProtoTreeItem(const google::protobuf::Descriptor *pclass, ProtoTreeItem *parentItem)
    : mName(pclass->name().c_str())
    , mType(proto::FieldDescriptor::TYPE_MESSAGE)
    , mLabel(proto::FieldDescriptor::LABEL_REQUIRED)
    , mDesc(pclass)
    , mParentItem(parentItem) {}

ProtoTreeItem::ProtoTreeItem(const google::protobuf::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : mName(field->name().c_str())
    , mType(field->type())
    , mLabel(field->label())
    , mDesc(field->message_type())
    , mParentItem(parentItem) {}

ProtoTreeItem::ProtoTreeItem(const QString& name, ProtoTreeItem *parentItem)
    : mName(name)
    , mType(proto::FieldDescriptor::TYPE_MESSAGE)
    , mLabel(proto::FieldDescriptor::LABEL_REQUIRED)
    , mDesc(nullptr)
    , mParentItem(parentItem) {}

ProtoTreeItem* ProtoTreeItem::child(size_t row)
{
    return row < mChildItems.size() ? mChildItems.at(row).get() : nullptr;
}

void ProtoTreeItem::expand()
{
    if(mDesc != nullptr && mChildItems.empty())
    {
        for(int i = 0; i < mDesc->field_count(); ++i)
        {
            mChildItems.push_back(std::make_unique<ProtoTreeItem>(mDesc->field(i), this));
        }
    }
}

int ProtoTreeItem::fieldCount() const
{
    return mDesc->field_count();
}

size_t ProtoTreeItem::childCount() const
{
    return mChildItems.size();
}

int ProtoTreeItem::columnCount()
{
    return 4;
}

QVariant ProtoTreeItem::data(int column) const
{
    switch(column)
    {
        case 0:
            return QVariant();
        case 1:
            return mName;
        case 2:
            return getTypeName();
        case 3:
            return mValue;
    }
    return QVariant();
}

QBrush ProtoTreeItem::color() const
{
    switch(mType)
    {
        case proto::FieldDescriptor::TYPE_DOUBLE:
        case proto::FieldDescriptor::TYPE_FLOAT:
            return QBrush(QColor(0, 0, 255, 90));
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
            return QBrush(QColor(0, 255, 0, 90));
        case proto::FieldDescriptor::TYPE_BOOL:
            return QBrush(QColor(255, 0, 255, 90));
        case proto::FieldDescriptor::TYPE_STRING:
            return QBrush(QColor(255, 255, 0, 90));
        case proto::FieldDescriptor::TYPE_GROUP:
        case proto::FieldDescriptor::TYPE_MESSAGE:
        case proto::FieldDescriptor::TYPE_BYTES:
            return QBrush(QColor(255, 0, 0, 90));
        case proto::FieldDescriptor::TYPE_ENUM:
            return QBrush(QColor(0, 255, 255, 90));
    }
    return Qt::white;
}

QString ProtoTreeItem::getTypeName() const
{
    switch(mType)
    {
        case proto::FieldDescriptor::TYPE_DOUBLE:
            return "Double";
        case proto::FieldDescriptor::TYPE_FLOAT:
            return "Float";
        case proto::FieldDescriptor::TYPE_INT64:
            return "Int64";
        case proto::FieldDescriptor::TYPE_UINT64:
            return "Uint64";
        case proto::FieldDescriptor::TYPE_INT32:
            return "Int32";
        case proto::FieldDescriptor::TYPE_FIXED64:
            return "Fixed64";
        case proto::FieldDescriptor::TYPE_FIXED32:
            return "Fixed32";
        case proto::FieldDescriptor::TYPE_BOOL:
            return "Boolean";
        case proto::FieldDescriptor::TYPE_STRING:
            return "String";
        case proto::FieldDescriptor::TYPE_GROUP:
            return "Group";
        case proto::FieldDescriptor::TYPE_MESSAGE:
            return "Message";
        case proto::FieldDescriptor::TYPE_BYTES:
            return "Bytes";
        case proto::FieldDescriptor::TYPE_UINT32:
            return "Uint32";
        case proto::FieldDescriptor::TYPE_ENUM:
            return "Enum";
        case proto::FieldDescriptor::TYPE_SFIXED32:
            return "SFixed32";
        case proto::FieldDescriptor::TYPE_SFIXED64:
            return "SFixed64";
        case proto::FieldDescriptor::TYPE_SINT32:
            return "SInt32";
        case proto::FieldDescriptor::TYPE_SINT64:
            return "SInt64";
    }
    return "";
}

QIcon ProtoTreeItem::icon() const
{
    switch (mLabel)
    {
        case proto::FieldDescriptor::LABEL_OPTIONAL:
            return QIcon(":/icons/optional.png");
        case proto::FieldDescriptor::LABEL_REPEATED:
            return QIcon(":/icons/repeated.png");
        case proto::FieldDescriptor::LABEL_REQUIRED:
            return QIcon(":/icons/required.png");
    }
    return QIcon();
}

size_t ProtoTreeItem::row() const
{
    if(mParentItem == nullptr)
        return 0;

    auto it = std::find_if(mParentItem->mChildItems.begin(), mParentItem->mChildItems.end(),
                           [this](auto&& val) { return val.get() == this; });
    return std::distance(mParentItem->mChildItems.begin(), it);
}

ProtoTreeItem *ProtoTreeItem::parentItem()
{
    return mParentItem;
}
