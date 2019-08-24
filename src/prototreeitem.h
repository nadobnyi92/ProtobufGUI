#ifndef PROTOTREEITEM_H
#define PROTOTREEITEM_H

#include <QVector>
#include <QVariant>

class ProtoTreeItem
{
public:
    ProtoTreeItem(const QVector<QVariant> &data, ProtoTreeItem *parentItem = nullptr);
    ~ProtoTreeItem();

    void appendChild(ProtoTreeItem *child);

    ProtoTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    ProtoTreeItem *parentItem();

private:
    QVector<ProtoTreeItem*> mChildItems;
    QVector<QVariant> mItemData;
    ProtoTreeItem *mParentItem;
};

#endif // PROTOTREEITEM_H
