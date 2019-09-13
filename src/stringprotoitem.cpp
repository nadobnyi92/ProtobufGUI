#include "stringprotoitem.h"

#include <QLineEdit>

StringProtoItem::ItemDelegate::ItemDelegate(QObject *parent)
    : QItemDelegate(parent) {}

QWidget *StringProtoItem::ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    return new QLineEdit(parent);
}

void StringProtoItem::ItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit * e = static_cast<QLineEdit*>(editor);
    e->setText(index.model()->data(index).toString());
}

void StringProtoItem::ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit * e = static_cast<QLineEdit*>(editor);
    model->setData(index, e->text());
}

StringProtoItem::StringProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : ProtoTreeItem(field, parentItem)
    , mDelegate(new ItemDelegate) {}

StringProtoItem::~StringProtoItem()
{
    delete mDelegate;
}

QItemDelegate *StringProtoItem::getDelegate() const
{
    return mDelegate;
}

QBrush StringProtoItem::color() const
{
    return QBrush(QColor(255, 255, 0, 90));
}

QString StringProtoItem::getTypeName() const
{
    return "String";
}