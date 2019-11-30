#include "floatprotoitem.h"

#include <QDoubleSpinBox>

FloatProtoItem::ItemDelegate::ItemDelegate(QObject *parent) : QItemDelegate(parent) {}

QWidget *FloatProtoItem::ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    return new QDoubleSpinBox(parent);
}

void FloatProtoItem::ItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDoubleSpinBox * e = static_cast<QDoubleSpinBox*>(editor);
    e->setValue(index.model()->data(index).toDouble());
}

void FloatProtoItem::ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox * e = static_cast<QDoubleSpinBox*>(editor);
    model->setData(index, e->value());
}

FloatProtoItem::FloatProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : ProtoTreeItem(field, parentItem), mDelegate(new ItemDelegate) {}

FloatProtoItem::~FloatProtoItem()
{
    delete mDelegate;
}


QItemDelegate *FloatProtoItem::getDelegate() const
{
    return mDelegate;
}

QBrush FloatProtoItem::color() const
{
    return QBrush(QColor(0, 0, 255, 90));
}


void FloatProtoItem::setFieldValue(google::protobuf::Message *message)
{
    message->GetReflection()->SetFloat(message, mField, value().toFloat());
}


void FloatProtoItem::addFieldValue(google::protobuf::Message * message, const google::protobuf::FieldDescriptor * desc)
{
    message->GetReflection()->AddFloat(message, desc, value().toFloat());
}
