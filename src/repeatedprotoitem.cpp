#include "repeatedprotoitem.h"

RepeatedProtoItem::RepeatedProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : ProtoTreeItem(field, parentItem), mField(field) {}

void RepeatedProtoItem::addItem()
{
    createSingleNode(mField);
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

