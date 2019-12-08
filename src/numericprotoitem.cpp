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


void NumericProtoItem::setFieldValue(google::protobuf::Message *message)
{
    switch (mField->type())
    {
        case proto::FieldDescriptor::TYPE_SINT64:
        case proto::FieldDescriptor::TYPE_SFIXED64:
        case proto::FieldDescriptor::TYPE_FIXED64:
        case proto::FieldDescriptor::TYPE_INT64:
                message->GetReflection()->SetInt64(message, mField, value().toInt());
        break;
        case proto::FieldDescriptor::TYPE_UINT64:
            message->GetReflection()->SetUInt64(message, mField, value().toInt());
        break;
        case proto::FieldDescriptor::TYPE_SINT32:
        case proto::FieldDescriptor::TYPE_SFIXED32:
        case proto::FieldDescriptor::TYPE_FIXED32:
        case proto::FieldDescriptor::TYPE_INT32:
            message->GetReflection()->SetInt32(message, mField, value().toInt());
        break;
        case proto::FieldDescriptor::TYPE_UINT32:
            message->GetReflection()->SetUInt32(message, mField, value().toInt());
        break;
        default:
            std::cout << "unsopported type\n";
    }
}


void NumericProtoItem::addFieldValue(google::protobuf::Message * message, const google::protobuf::FieldDescriptor * desk)
{
    switch (mField->type())
    {
        case proto::FieldDescriptor::TYPE_SINT64:
        case proto::FieldDescriptor::TYPE_SFIXED64:
        case proto::FieldDescriptor::TYPE_FIXED64:
        case proto::FieldDescriptor::TYPE_INT64:
            message->GetReflection()->AddInt64(message, desk, value().toInt());
        break;
        case proto::FieldDescriptor::TYPE_UINT64:
            message->GetReflection()->AddUInt64(message, desk, value().toUInt());
        break;
        case proto::FieldDescriptor::TYPE_SINT32:
        case proto::FieldDescriptor::TYPE_SFIXED32:
        case proto::FieldDescriptor::TYPE_FIXED32:
        case proto::FieldDescriptor::TYPE_INT32:
            message->GetReflection()->AddInt32(message, desk, value().toInt());
        break;
        case proto::FieldDescriptor::TYPE_UINT32:
            message->GetReflection()->AddUInt32(message, desk, value().toUInt());
        break;
        default:
            std::cout << "unsopported type\n";
    }
}


void NumericProtoItem::initFieldValue(const google::protobuf::Message * message)
{
    switch (mField->type())
    {
        case proto::FieldDescriptor::TYPE_SINT64:
        case proto::FieldDescriptor::TYPE_SFIXED64:
        case proto::FieldDescriptor::TYPE_FIXED64:
        case proto::FieldDescriptor::TYPE_INT64:
            setData(static_cast<int>(message->GetReflection()->GetInt64(*message, mField)));
        break;
        case proto::FieldDescriptor::TYPE_UINT64:
            setData(static_cast<int>(message->GetReflection()->GetUInt64(*message, mField)));
        break;
        case proto::FieldDescriptor::TYPE_SINT32:
        case proto::FieldDescriptor::TYPE_SFIXED32:
        case proto::FieldDescriptor::TYPE_FIXED32:
        case proto::FieldDescriptor::TYPE_INT32:
            setData(message->GetReflection()->GetInt32(*message, mField));
        break;
        case proto::FieldDescriptor::TYPE_UINT32:
            setData(message->GetReflection()->GetUInt32(*message, mField));
        break;
        default:
            std::cout << "unsopported type\n";
    }
}
