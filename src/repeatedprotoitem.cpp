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

QVariant RepeatedProtoItem::data(int column) const
{
    QVariant data = ProtoTreeItem::data(column);
    if(column == 1)
    {
        data = data.toString().append("[").append(QString::number(rowCount())).append("]");
    }
    return data;
}

