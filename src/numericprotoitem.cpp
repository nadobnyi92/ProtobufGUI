#include "numericprotoitem.h"

#include <QSpinBox>

NumericProtoItem::ItemDelegate::ItemDelegate(QObject *parent)
    : QItemDelegate(parent) {}

QWidget *NumericProtoItem::ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    return new QSpinBox(parent);
}

void NumericProtoItem::ItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QSpinBox * e = static_cast<QSpinBox*>(editor);
    e->setValue(index.model()->data(index).toInt());
}

void NumericProtoItem::ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox * e = static_cast<QSpinBox*>(editor);
    model->setData(index, e->value());
}

NumericProtoItem::NumericProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : ProtoTreeItem(field, parentItem)
    , mDelegate(new ItemDelegate) {}

NumericProtoItem::~NumericProtoItem()
{
    delete mDelegate;
}

QBrush NumericProtoItem::color() const
{
    return QBrush(QColor(0, 255, 0, 90));
}

QItemDelegate *NumericProtoItem::getDelegate() const
{
    return mDelegate;
}

QString NumericProtoItem::getTypeName() const
{
    switch(type())
    {
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
        case proto::FieldDescriptor::TYPE_UINT32:
            return "Uint32";
        case proto::FieldDescriptor::TYPE_SFIXED32:
            return "SFixed32";
        case proto::FieldDescriptor::TYPE_SFIXED64:
            return "SFixed64";
        case proto::FieldDescriptor::TYPE_SINT32:
            return "SInt32";
        case proto::FieldDescriptor::TYPE_SINT64:
            return "SInt64";
        default:
            return "";
    }
}
