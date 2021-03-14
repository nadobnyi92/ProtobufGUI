#include "prototreeitem.h"

#include<algorithm>

#include <QIcon>
#include <QPainter>
#include <QDebug>

#include "numericprotoitem.h"
#include "floatprotoitem.h"
#include "stringprotoitem.h"
#include "boolprotoitem.h"
#include "enumprotoitem.h"
#include "repeatedprotoitem.h"
#include "messageprotoitem.h"
#include "bytestprotoitem.h"
#include "../protobufmodel.h"

ProtoTreeItem::ProtoTreeItem(ProtobufModel& model, const google::protobuf::Descriptor *pclass, ProtoTreeItem *parentItem)
    : QObject(parentItem)
    , mField(nullptr)
    , mName(pclass->name().c_str())
    , mDesc(pclass)
    , mModel(model)
    , mParentItem(parentItem)
{
    init();
}

ProtoTreeItem::ProtoTreeItem(ProtobufModel& model, const google::protobuf::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : QObject(parentItem)
    , mField(field)
    , mName(field->name().c_str())
    , mDesc(field->message_type())
    , mModel(model)
    , mParentItem(parentItem)
{
    init();
}

ProtoTreeItem::~ProtoTreeItem() {}

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

void ProtoTreeItem::addAction(QAction *action)
{
    mActions.append(action);
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

ProtobufModel &ProtoTreeItem::model()
{
    return mModel;
}

void ProtoTreeItem::init()
{
    if(mParentItem && mParentItem->isRepeated())
    {
        QAction *act = new QAction(this);
        act->setText("Удалить");
        connect(act, &QAction::triggered, [&]() {
            model().beginRemoveItem(mParentItem);
            mParentItem->mChildItems.erase(
                std::remove(mParentItem->mChildItems.begin(),
                            mParentItem->mChildItems.end(),
                            this));
            model().endRemoveItem(mParentItem);
        });
        addAction(act);
    }
}

const google::protobuf::Descriptor *ProtoTreeItem::descriptor() const
{
    return mDesc;
}

const std::vector<ProtoTreeItem*> &ProtoTreeItem::children() const
{
    return mChildItems;
}

const google::protobuf::FieldDescriptor *ProtoTreeItem::field() const
{
    return mField;
}

ProtoTreeItem* ProtoTreeItem::createRepeatedNode(const google::protobuf::FieldDescriptor *field)
{
    mChildItems.push_back(new RepeatedProtoItem(model(), field, this));
    return mChildItems.back();
}

ProtoTreeItem* ProtoTreeItem::createNode(const google::protobuf::FieldDescriptor *field)
{
    switch(field->type())
    {
        case proto::FieldDescriptor::TYPE_DOUBLE:
        case proto::FieldDescriptor::TYPE_FLOAT:
            mChildItems.push_back(new FloatProtoItem(model(), field, this));
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
            mChildItems.push_back(new NumericProtoItem(model(), field, this));
            break;
        case proto::FieldDescriptor::TYPE_BOOL:
            mChildItems.push_back(new BoolProtoItem(model(), field, this));
            break;
        case proto::FieldDescriptor::TYPE_BYTES:
            mChildItems.push_back(new BytesProtoItem(model(), field, this));
        break;
        case proto::FieldDescriptor::TYPE_STRING:
            mChildItems.push_back(new StringProtoItem(model(), field, this));
            break;
        case proto::FieldDescriptor::TYPE_GROUP:
        case proto::FieldDescriptor::TYPE_MESSAGE:
            mChildItems.push_back(new MessageProtoItem(model(), field, this));
            break;
        case proto::FieldDescriptor::TYPE_ENUM:
            mChildItems.push_back(new EnumProtoItem(model(), field, this));
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

const QList<QAction *> &ProtoTreeItem::actions() const
{
    return mActions;
}

ProtoTreeItem *ProtoTreeItem::parentItem()
{
    return mParentItem;
}
