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
    ProtoTreeItem(const QString& name, ProtoTreeItem *parentItem = nullptr);

    virtual ~ProtoTreeItem() {}

    int fieldCount() const;
    int rowCount() const;
    ProtoTreeItem *parentItem();
    ProtoTreeItem *child(size_t row);
    size_t row() const;

    virtual QVariant data(int column) const;

    QString name() const;
    QVariant value() const;
    proto::FieldDescriptor::Type type() const;
    QString typeName() const;
    proto::FieldDescriptor::Label label() const;

    virtual void expand();
    void setData(const QVariant& data);

    virtual QItemDelegate* getDelegate() const;
    virtual QBrush color() const;

protected:
    void createSingleNode(const proto::FieldDescriptor * field);

private:
    void expandChildren();
    void createNode(const proto::FieldDescriptor * field);

private:
    QString mName;
    QString mTypeName;
    proto::FieldDescriptor::Type mType;
    proto::FieldDescriptor::Label mLabel;
    QVariant mValue;
    const google::protobuf::Descriptor * mDesc;

    std::vector< std::unique_ptr<ProtoTreeItem> > mChildItems;
    ProtoTreeItem *mParentItem;

};

#endif // PROTOTREEITEM_H
