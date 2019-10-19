#ifndef NUMERICPROTOITEM_H
#define NUMERICPROTOITEM_H

#include "prototreeitem.h"

#include <QItemDelegate>

class NumericProtoItem : public ProtoTreeItem
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
    NumericProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem = nullptr);
    ~NumericProtoItem() override;
    // ProtoTreeItem interface
public:
    QBrush color() const override;
    QItemDelegate *getDelegate() const override;

private:
    ItemDelegate* mDelegate;

    // ProtoTreeItem interface
protected:
    void setFieldValue(google::protobuf::Message *message) override;
};

#endif // NUMERICPROTOITEM_H
