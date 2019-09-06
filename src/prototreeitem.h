#ifndef PROTOTREEITEM_H
#define PROTOTREEITEM_H

#include <QVariant>
#include <QBrush>
#include <QIcon>

#include <vector>
#include <bits/unique_ptr.h>

#include "protodata.h"

class ProtoTreeItem
{
public:
    ProtoTreeItem(const google::protobuf::Descriptor *protoclass, ProtoTreeItem *parentItem = nullptr);
    ProtoTreeItem(const google::protobuf::FieldDescriptor * field, ProtoTreeItem *parentItem = nullptr);
    ProtoTreeItem(const QString& name, ProtoTreeItem *parentItem = nullptr); // : name(n), mDesc(nullptr) {}

    //ProtoTreeItem(const ProtoData &data, ProtoTreeItem *parentItem = nullptr);

    size_t childCount() const;
    int fieldCount() const;
    static int columnCount();
    QVariant data(int column) const;
    QBrush color() const;
    QIcon icon() const;
    size_t row() const;
    ProtoTreeItem *parentItem();

    ProtoTreeItem *child(size_t row);

    void expand();

private:
    QString getTypeName() const;

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
