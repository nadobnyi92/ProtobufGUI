#include "boolprotoitem.h"

#include <QCheckBox>

BoolProtoItem::ItemDelegate::ItemDelegate(QObject *parent)
    : QItemDelegate(parent) {}

QWidget *BoolProtoItem::ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    return new QCheckBox(parent);
}

void BoolProtoItem::ItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QCheckBox * e = static_cast<QCheckBox*>(editor);
    e->setChecked(index.model()->data(index).toString() == "True");
}

void BoolProtoItem::ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QCheckBox * e = static_cast<QCheckBox*>(editor);
    model->setData(index, e->checkState() == Qt::Checked ? "True" : "False");
}

BoolProtoItem::BoolProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem)
    :ProtoTreeItem(field, parentItem), mDelegate(new ItemDelegate) {}

BoolProtoItem::~BoolProtoItem()
{
    delete mDelegate;
}


QItemDelegate *BoolProtoItem::getDelegate() const
{
    return mDelegate;
}

QBrush BoolProtoItem::color() const
{
    return QBrush(QColor(255, 0, 255, 90));
}


void BoolProtoItem::setFieldValue(google::protobuf::Message *message)
{
    message->GetReflection()->SetBool(message, mField, value().toString() == "True");
}


void BoolProtoItem::addFieldValue(google::protobuf::Message * message, const google::protobuf::FieldDescriptor * desc)
{
    message->GetReflection()->AddBool(message, mField, value().toString() == "True");
}


void BoolProtoItem::initFieldValue(const google::protobuf::Message * message)
{
    setData(message->GetReflection()->GetBool(*message, mField));
}
