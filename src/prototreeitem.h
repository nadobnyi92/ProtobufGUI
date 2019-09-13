#ifndef PROTOTREEITEM_H
#define PROTOTREEITEM_H

#include <QVariant>
#include <QBrush>
#include <QIcon>

#include <vector>
#include <bits/unique_ptr.h>

#include <google/protobuf/dynamic_message.h>

namespace proto = google::protobuf;

class QItemDelegate;

class ProtoTreeItem
{
public:
    ProtoTreeItem(const proto::Descriptor *protoclass, ProtoTreeItem *parentItem = nullptr);
    ProtoTreeItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem = nullptr);
    ProtoTreeItem(const QString& name, ProtoTreeItem *parentItem = nullptr);

    virtual ~ProtoTreeItem() {}

    size_t childCount() const;
    int fieldCount() const;
    static int columnCount();
    QVariant data(int column) const;
    QIcon icon() const;
    size_t row() const;
    ProtoTreeItem *parentItem();
    ProtoTreeItem *child(size_t row);
    Qt::ItemFlags flags() const;

    void expand();
    void setData(const QVariant& data);

    virtual QItemDelegate* getDelegate() const;
    virtual QBrush color() const;

protected:
    proto::FieldDescriptor::Type type() const;
    virtual QString getTypeName() const;

private:
    void expandChildren();
    void createNode(const proto::FieldDescriptor * field);

private:
    QString mName;
    proto::FieldDescriptor::Type mType;
    proto::FieldDescriptor::Label mLabel;
    QVariant mValue;
    const google::protobuf::Descriptor * mDesc;

    std::vector< std::unique_ptr<ProtoTreeItem> > mChildItems;
    ProtoTreeItem *mParentItem;

};

#endif // PROTOTREEITEM_H
