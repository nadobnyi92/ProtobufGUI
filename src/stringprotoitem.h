#ifndef STRINGPROTOITEM_H
#define STRINGPROTOITEM_H

#include "prototreeitem.h"

#include <QItemDelegate>

class StringProtoItem : public ProtoTreeItem
{
    class ItemDelegate : public QItemDelegate
    {
        public:
            ItemDelegate(QObject * parent = nullptr);

            QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
            void setEditorData(QWidget *editor, const QModelIndex &index) const override;
            void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    };

public:
    StringProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem = nullptr);
    ~StringProtoItem() override;

    // ProtoTreeItem interface
public:
    QItemDelegate *getDelegate() const override;
    QBrush color() const override;

private:
    ItemDelegate *mDelegate;
};

#endif // STRINGPROTOITEM_H
