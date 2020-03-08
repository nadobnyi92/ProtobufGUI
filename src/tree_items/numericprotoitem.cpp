#include "numericprotoitem.h"

#include <any>

#include <QSpinBox>

#include "protofieldwrapper.h"

createWrapper(proto::int32, Int32, INT32)
createWrapper(proto::int64, Int64, INT64)
createWrapper(proto::uint32, UInt32, UINT32)
createWrapper(proto::uint64, UInt64, UINT64)

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

void NumericProtoItem::fillFieldValue(google::protobuf::Message *message)
{
    auto fw = FieldWrapperRegister::get().wrapper(field()->cpp_type());
    if(fw != nullptr) {
        fw->setFieldData(message, field(), value());
    }
}

void NumericProtoItem::addFieldValue(google::protobuf::Message * message, const google::protobuf::FieldDescriptor * desk)
{
    auto fw = FieldWrapperRegister::get().wrapper(desk->cpp_type());
    if(fw != nullptr) {
        fw->setRepeatedFieldData(message, desk, value());
    }
}

void NumericProtoItem::initFieldValue(const google::protobuf::Message * message)
{
    auto fw = FieldWrapperRegister::get().wrapper(field()->cpp_type());
    if(fw != nullptr) {
        setValue(fw->getFieldData(*message, field()));
    }
}

void NumericProtoItem::initRepeatedFieldValue(const google::protobuf::Message * message, int idx)
{
    auto fw = FieldWrapperRegister::get().wrapper(field()->cpp_type());
    if(fw != nullptr) {
        setValue(fw->getRepeatedFieldData(*message, field(), idx));
    }
}
