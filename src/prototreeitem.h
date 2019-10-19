#ifndef PROTOTREEITEM_H
#define PROTOTREEITEM_H

#include <QVariant>
#include <QBrush>
#include <QIcon>
#include <QMenu>

#include <vector>
#include <bits/unique_ptr.h>

#include <google/protobuf/dynamic_message.h>

namespace proto = google::protobuf;

class QItemDelegate;
class ProtoManager;

class ProtoTreeItem : public QObject
{
    Q_OBJECT

public:
    ProtoTreeItem(const proto::Descriptor *protoclass, ProtoTreeItem *parentItem = nullptr);
    ProtoTreeItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem = nullptr);

    virtual ~ProtoTreeItem() {}

    int fieldCount() const;
    int rowCount() const;
    ProtoTreeItem *parentItem();
    ProtoTreeItem *child(size_t row);
    size_t row() const;

    virtual QString name() const;
    QVariant value() const;
    proto::FieldDescriptor::Type type() const;
    QString typeName() const;
    proto::FieldDescriptor::Label label() const;

    virtual void expand();
    void setData(const QVariant& data);
    void setDesc(const proto::Descriptor * desc);
    void removeRow(int row);

    virtual QItemDelegate* getDelegate() const;
    virtual QBrush color() const;

protected:
    void createSingleNode(const proto::FieldDescriptor * field);
    void expandChildren();
    virtual void setFieldValue(proto::Message* message);
    const google::protobuf::Descriptor * descriptor() const;
    const std::vector< std::unique_ptr<ProtoTreeItem> >& childItems() const;

protected:
    const proto::FieldDescriptor * mField;


private:
    void createNode(const proto::FieldDescriptor * field);

private:
    QString mName;
    QString mTypeName;
    proto::FieldDescriptor::Type mType;
    proto::FieldDescriptor::Label mLabel;
    QVariant mValue;
    const proto::Descriptor * mDesc;

    std::vector< std::unique_ptr<ProtoTreeItem> > mChildItems;
    ProtoTreeItem *mParentItem;
};

#endif // PROTOTREEITEM_H
