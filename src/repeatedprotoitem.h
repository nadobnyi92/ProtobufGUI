#ifndef REPEATEDPROTOITEM_H
#define REPEATEDPROTOITEM_H

#include "prototreeitem.h"

class RepeatedProtoItem : public ProtoTreeItem
{
    Q_OBJECT

public:
    RepeatedProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem = nullptr);

public slots:
    void addItem();

signals:
    void onItemsAdded(const QModelIndex&);

    // ProtoTreeItem interface
public:
    QBrush color() const override;
    QString name() const override;
    QItemDelegate *getDelegate() const override;

protected:
    void setFieldValue(google::protobuf::Message *message) override;
    void addFieldValue(google::protobuf::Message *, const google::protobuf::FieldDescriptor *) override {}
};

#endif // REPEATEDPROTOITEM_H
