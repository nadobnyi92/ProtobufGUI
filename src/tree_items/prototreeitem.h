#ifndef PROTOTREEITEM_H
#define PROTOTREEITEM_H

#include <QAbstractItemModel>
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

    ProtoTreeItem *parentItem();
    const std::vector< std::unique_ptr<ProtoTreeItem> >& children() const;

    QString name() const;
    QVariant value() const;
    proto::FieldDescriptor::Type type() const;
    QString typeName() const;
    proto::FieldDescriptor::Label label() const;

    void expand();
    void setValue(const QVariant& data);
    void setDesc(const proto::Descriptor * desc); //TODO hide it
    void removeRow(int row); //TODO hide it

    virtual QItemDelegate* getDelegate() const = 0;
    virtual QBrush color() const = 0;

    virtual void fillFieldValue(proto::Message*) = 0;
    virtual void addFieldValue(proto::Message*, const proto::FieldDescriptor*) = 0;
    virtual void initFieldValue(const proto::Message*) = 0;
    virtual void initRepeatedFieldValue(const proto::Message*, int idx) = 0;
    virtual void clearValue();

    const google::protobuf::Descriptor * descriptor() const;
    const proto::FieldDescriptor * field() const;

protected:
    std::unique_ptr<ProtoTreeItem>& createNode(const proto::FieldDescriptor * field);
    std::unique_ptr<ProtoTreeItem>& createRepeatedNode(const proto::FieldDescriptor * field);

    void expandChildren();
    void clearChildren();

private:
    bool isRepeated(const google::protobuf::FieldDescriptor *field) const;

private:
    const proto::FieldDescriptor * mField;
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
