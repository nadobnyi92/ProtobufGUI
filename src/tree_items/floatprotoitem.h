#ifndef FLOATPROTOITEM_H
#define FLOATPROTOITEM_H

#include <QItemDelegate>

#include "prototreeitem.h"

class FloatProtoItem : public ProtoTreeItem
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
    FloatProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem = nullptr);
    ~FloatProtoItem() override;

    QItemDelegate *getDelegate() const override;

protected:
    void fillFieldValue(google::protobuf::Message *message) override;
    void addFieldValue(google::protobuf::Message *, const google::protobuf::FieldDescriptor *) override;
    void initFieldValue(const google::protobuf::Message *) override;
    void initRepeatedFieldValue(const google::protobuf::Message *, int idx) override;

private:
    ItemDelegate *mDelegate;

};

#endif // FLOATPROTOITEM_H
