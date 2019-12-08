#include "repeatedprotoitem.h"

RepeatedProtoItem::RepeatedProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : ProtoTreeItem(field, parentItem) {}

void RepeatedProtoItem::addItem()
{
    createSingleNode(mField);
    expandChildren();
}

QBrush RepeatedProtoItem::color() const
{
    return QBrush(QColor(255, 0, 0, 90));
}

QString RepeatedProtoItem::name() const
{
    return ProtoTreeItem::name()
        .append("[").append(QString::number(rowCount())).append("]");
}

QItemDelegate *RepeatedProtoItem::getDelegate() const
{
    return nullptr;
}

void RepeatedProtoItem::setFieldValue(google::protobuf::Message *message)
{
    for(auto& child: childItems())
    {
        child->addFieldValue(message, mField);
    }
}


void RepeatedProtoItem::initFieldValue(const google::protobuf::Message *)
{

}
