#include "enumprotoitem.h"
#include "../prototreeerror.h"

#include <sstream>

#include <QComboBox>

EnumProtoItem::ItemDelegate::ItemDelegate(const QStringList& items,QObject *parent)
    : QItemDelegate(parent), mItems(items) {}

QWidget *EnumProtoItem::ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    return new QComboBox(parent);
}

void EnumProtoItem::ItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox * e = static_cast<QComboBox*>(editor);
    e->addItems(mItems);
    e->setCurrentText(index.model()->data(index).toString());
}

void EnumProtoItem::ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox * e = static_cast<QComboBox*>(editor);
    model->setData(index, e->currentText());
}

EnumProtoItem::EnumProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : ProtoTreeItem(field, parentItem)
{
    QStringList items;
    for(int i = 0, c = field->enum_type()->value_count(); i < c; ++i)
        items.append(field->enum_type()->value(i)->name().c_str());
    mDelegate = new ItemDelegate(items);
}

EnumProtoItem::~EnumProtoItem()
{
    delete mDelegate;
}

QItemDelegate *EnumProtoItem::getDelegate() const
{
    return mDelegate;
}

QBrush EnumProtoItem::color() const
{
    return QBrush(QColor(0, 255, 255, 90));
}

int EnumProtoItem::getEnumValue()
{
    auto enumType = field()->enum_type()->FindValueByName(value().toString().toStdString());
    if(enumType == nullptr)
    {
        std::stringstream ss;
        ss << "Field " << value().toString().toStdString()
           << " is not represend in enum " << field()->name();
        throw ProtoTreeError("Failed init enum field", ss.str());
    }
    return enumType->number();
}

void EnumProtoItem::fillFieldValue(google::protobuf::Message *message)
{
    message->GetReflection()->SetEnumValue(message, field(), getEnumValue());
}

void EnumProtoItem::addFieldValue(google::protobuf::Message *message, const google::protobuf::FieldDescriptor *desc)
{
    message->GetReflection()->AddEnumValue(message, desc, getEnumValue());
}

void EnumProtoItem::initFieldValue(const google::protobuf::Message * message)
{
    setValue(QString::fromStdString(message->GetReflection()->GetEnum(*message, field())->name()));
}

void EnumProtoItem::initRepeatedFieldValue(const google::protobuf::Message * message, int idx)
{
    setValue(QString::fromStdString(message->GetReflection()->GetRepeatedEnum(*message, field(), idx)->name()));
}

