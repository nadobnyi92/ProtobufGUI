#ifndef PROTOTREEITEM_H
#define PROTOTREEITEM_H

#include <QVariant>
#include <QBrush>
#include <QIcon>
#include <QMenu>
#include <QQmlListProperty>

#include <vector>
#include <bits/unique_ptr.h>

#include <google/protobuf/dynamic_message.h>

namespace proto = google::protobuf;

class QItemDelegate;
class ProtoManager;

class ProtoTreeItem : public QObject
{
    Q_OBJECT

//    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
//    Q_PROPERTY(QVariant value READ svalue WRITE setSValue NOTIFY valueChanged)
//    Q_PROPERTY(QQmlListProperty<ProtoTreeItem> children READ children NOTIFY childrenChanged)
//    Q_PROPERTY(int label READ label WRITE setLabel CONSTANT)

public:
    ProtoTreeItem(const proto::Descriptor *protoclass, ProtoTreeItem *parentItem = nullptr);
    ProtoTreeItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem = nullptr);

    virtual ~ProtoTreeItem() {}

    int rowCount() const;
    ProtoTreeItem *parentItem();
    ProtoTreeItem *child(size_t row);
    int row() const;

    QString name() const;
    QVariant value() const;
    proto::FieldDescriptor::Type type() const;
    QString typeName() const;
    proto::FieldDescriptor::Label label() const;

    QQmlListProperty<ProtoTreeItem> children();
    int childCount();
    ProtoTreeItem* childNode(int idx);

    void expand();
    void setValue(const QVariant& data);
    void setName(const QString& name);
    void setDesc(const proto::Descriptor * desc);
    void removeRow(int row);

    virtual QItemDelegate* getDelegate() const = 0;
    virtual QBrush color() const = 0;
    virtual void fillFieldValue(proto::Message*) = 0;
    virtual void addFieldValue(proto::Message*, const proto::FieldDescriptor*) = 0;
    virtual void initFieldValue(const proto::Message*) = 0;
    virtual void initRepeatedFieldValue(const proto::Message*, int idx) = 0;
    virtual void clearValue();
    virtual QList<QAction*> getActions();

    const google::protobuf::Descriptor * descriptor() const;
    const std::vector< std::unique_ptr<ProtoTreeItem> >& childItems() const;
    const proto::FieldDescriptor * field() const;

protected:
    std::unique_ptr<ProtoTreeItem>& createNode(const proto::FieldDescriptor * field, bool isRepeated = true);
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
