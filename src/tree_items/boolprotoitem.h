#ifndef BOOLPROTOITEM_H
#define BOOLPROTOITEM_H

#include <QItemDelegate>

#include "prototreeitem.h"

class BoolProtoItem : public ProtoTreeItem
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
    BoolProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem = nullptr);
    ~BoolProtoItem() override;

    // ProtoTreeItem interface
public:
    QItemDelegate *getDelegate() const override;
    QBrush color() const override;
    void initFieldValue(const google::protobuf::Message *) override;

    // ProtoTreeItem interface
protected:
    void setFieldValue(google::protobuf::Message *message) override;
    void addFieldValue(google::protobuf::Message *, const google::protobuf::FieldDescriptor *) override;

private:
    ItemDelegate *mDelegate;
};

#endif // BOOLPROTOITEM_H