#include "numericprotoitem.h"

#include <QSpinBox>

#include <variant>

template <typename T>
struct IntegerType
{
    T(proto::Reflection::*get_int)(const proto::Message&, const proto::FieldDescriptor*) const;
    T(proto::Reflection::*get_repeated_int)(const proto::Message&, const proto::FieldDescriptor*, int) const;
    void(proto::Reflection::*set_int)(proto::Message*, const proto::FieldDescriptor*, T) const;
    void(proto::Reflection::*set_repeated_int)(proto::Message*, const proto::FieldDescriptor*, T) const;
    int minValue;
    int maxValue;
};

#define createIntegerType(N1, N2) \
    IntegerType<proto::N1> {&proto::Reflection::Get##N2, \
                            &proto::Reflection::GetRepeated##N2, \
                            &proto::Reflection::Set##N2, \
                            &proto::Reflection::Add##N2}

using IType = std::variant<
    IntegerType<proto::int32>,
    IntegerType<proto::int64>,
    IntegerType<proto::uint32>,
    IntegerType<proto::uint64> >;

static std::map<proto::FieldDescriptor::CppType, IType> mTypes {
    { proto::FieldDescriptor::CPPTYPE_INT32, createIntegerType(int32, Int32) },
    { proto::FieldDescriptor::CPPTYPE_INT64, createIntegerType(int64, Int64) },
    { proto::FieldDescriptor::CPPTYPE_UINT32, createIntegerType(uint32, UInt32) },
    { proto::FieldDescriptor::CPPTYPE_UINT64, createIntegerType(uint64, UInt64) }
};

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
    auto func = [&](auto& a){
        (message->GetReflection()->*a.set_int)(message, field(), value().toInt());
    };
    std::visit(func, mTypes[field()->cpp_type()]);
}


void NumericProtoItem::addFieldValue(google::protobuf::Message * message, const google::protobuf::FieldDescriptor * desk)
{
    auto func = [&](auto& a){
        (message->GetReflection()->*a.set_repeated_int)(message, desk, value().toInt());
    };
    std::visit(func, mTypes[desk->cpp_type()]);
}


void NumericProtoItem::initFieldValue(const google::protobuf::Message * message)
{
    auto func = [&](auto& a){
        auto res = (message->GetReflection()->*a.get_int)(*message, field());
        return QVariant::fromValue(res);
    };
    setValue(std::visit(func, mTypes[field()->cpp_type()]));
}


void NumericProtoItem::initRepeatedFieldValue(const google::protobuf::Message * message, int idx)
{
    auto func = [&](auto& a) {
        auto res = (message->GetReflection()->*a.get_repeated_int)(*message, field(), idx);
        return QVariant::fromValue(res);
    };
    setValue(std::visit(func, mTypes[field()->cpp_type()]));
}
