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

int count_element(QQmlListProperty<ProtoTreeItem> *property)
{
    ProtoTreeItem *parent = (qobject_cast<ProtoTreeItem *>(property->object));
    return parent->childCount();
}

ProtoTreeItem *at_element(QQmlListProperty<ProtoTreeItem> *property, int index)
{
    ProtoTreeItem *parent = (qobject_cast<ProtoTreeItem *>(property->object));
    if(index < 0 || index >= parent->childCount())
        return nullptr;
    return parent->childNode(index);
}


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
    if(parentItem != nullptr && parentItem->label() == proto::FieldDescriptor::LABEL_REPEATED) {
        mLabel = proto::FieldDescriptor::LABEL_OPTIONAL;
    } else {
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
        child->expandChildren();
}

void ProtoTreeItem::expandChildren()
{
    if( mLabel != proto::FieldDescriptor::LABEL_REPEATED &&
        mDesc != nullptr &&
        mChildItems.empty() ) {
        for(int i = 0; i < mDesc->field_count(); ++i)
            createNode(mDesc->field(i));
    }
}

void ProtoTreeItem::clearChildren()
{
    mChildItems.clear();
}

const google::protobuf::Descriptor *ProtoTreeItem::descriptor() const
{
    return mDesc;
}

const std::vector<std::unique_ptr<ProtoTreeItem> > &ProtoTreeItem::childItems() const
{
    return mChildItems;
}

//ProtoTreeItem *ProtoTreeItem::item(int idx)
//{
//    return mChildItems.at(idx);
//}

const google::protobuf::FieldDescriptor *ProtoTreeItem::field() const
{
    return mField;
}

std::unique_ptr<ProtoTreeItem>& ProtoTreeItem::createNode(const google::protobuf::FieldDescriptor *field, bool isRepeat)
{
    if(field->label() == proto::FieldDescriptor::LABEL_REPEATED && isRepeat) {
        mChildItems.push_back(std::unique_ptr<RepeatedProtoItem>(new RepeatedProtoItem(field, this)));
    } else {
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
        }
    }
    return mChildItems.back();
}

void ProtoTreeItem::setValue(const QVariant &data)
{
    mValue = data;
}

void ProtoTreeItem::setName(const QString &name)
{
    mName = name;
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

QList<QAction *> ProtoTreeItem::getActions()
{
    return QList<QAction*>();
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
        tName.append(QString("(%1::%2)")
                        .arg(mDesc->file()->package().c_str())
                        .arg(mDesc->name().c_str()));
    return tName;
}

google::protobuf::FieldDescriptor::Label ProtoTreeItem::label() const
{
    return mLabel;
}

QQmlListProperty<ProtoTreeItem> ProtoTreeItem::children()
{
   QQmlListProperty<ProtoTreeItem> items(this, 0,
                                          &count_element,
                                          &at_element);
   return items;
}

int ProtoTreeItem::childCount()
{
    return mChildItems.size();
}

ProtoTreeItem *ProtoTreeItem::childNode(int idx)
{
    return mChildItems.at(idx).get();
}

int ProtoTreeItem::row() const
{
    if(mParentItem == nullptr)
        return 0;
    auto it = std::find_if(mParentItem->mChildItems.begin(), mParentItem->mChildItems.end(),
        [this](const std::unique_ptr<ProtoTreeItem>& val) { return val.get() == this; });
    return static_cast<int>(std::distance(mParentItem->mChildItems.begin(), it));
}

ProtoTreeItem *ProtoTreeItem::parentItem()
{
    return mParentItem;
}
