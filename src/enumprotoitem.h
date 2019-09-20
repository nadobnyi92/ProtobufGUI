#ifndef ENUMPROTOITEM_H
#define ENUMPROTOITEM_H

#include <QItemDelegate>

#include "prototreeitem.h"

class EnumProtoItem : public ProtoTreeItem
{
    class ItemDelegate : public QItemDelegate
    {
    public:
        ItemDelegate(const QStringList& items, QObject * parent = nullptr);

        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
        void setEditorData(QWidget *editor, const QModelIndex &index) const override;
        void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    private:
        QStringList mItems;
    };

public:
    EnumProtoItem( const proto::FieldDescriptor * field, ProtoTreeItem *parentItem = nullptr);
    ~EnumProtoItem() override;
    // ProtoTreeItem interface
public:
    QItemDelegate *getDelegate() const override;
    QBrush color() const override;

private:
    ItemDelegate *mDelegate;
};

#endif // ENUMPROTOITEM_H
