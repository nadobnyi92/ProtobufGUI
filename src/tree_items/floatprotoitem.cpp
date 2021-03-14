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

FloatProtoItem::FloatProtoItem(ProtobufModel& model, const proto::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : ProtoTreeItem(model, field, parentItem), mDelegate(new ItemDelegate) {}

FloatProtoItem::~FloatProtoItem()
{
    delete mDelegate;
}


QItemDelegate *FloatProtoItem::getDelegate() const
{
    return mDelegate;
}

void FloatProtoItem::fillFieldValue(google::protobuf::Message *message)
{
    message->GetReflection()->SetFloat(message, field(), value().toFloat());
}


void FloatProtoItem::addFieldValue(google::protobuf::Message * message, const google::protobuf::FieldDescriptor * desc)
{
    message->GetReflection()->AddFloat(message, desc, value().toFloat());
}


void FloatProtoItem::initFieldValue(const google::protobuf::Message * message)
{
    setValue(message->GetReflection()->GetFloat(*message, field()));
}


void FloatProtoItem::initRepeatedFieldValue(const google::protobuf::Message *message, int idx)
{
    setValue(message->GetReflection()->GetRepeatedFloat(*message, field(), idx));
}
