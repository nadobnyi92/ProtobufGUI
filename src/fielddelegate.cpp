#include "fielddelegate.h"

#include <QLineEdit>

#include "tree_items/prototreeitem.h"

FieldDelegate::FieldDelegate(QObject * parent)
    : QItemDelegate(parent) {}


QWidget *FieldDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& style, const QModelIndex& index) const
{
    ProtoTreeItem *item = static_cast<ProtoTreeItem*>(index.internalPointer());
    return item->getDelegate()->createEditor(parent, style, index);
}

void FieldDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    ProtoTreeItem *item = static_cast<ProtoTreeItem*>(index.internalPointer());
    item->getDelegate()->setEditorData(editor, index);
}

void FieldDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    ProtoTreeItem *item = static_cast<ProtoTreeItem*>(index.internalPointer());
    item->getDelegate()->setModelData(editor, model, index);
}
