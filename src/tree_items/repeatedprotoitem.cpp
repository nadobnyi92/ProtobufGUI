#include "repeatedprotoitem.h"
#include "../protobufmodel.h"

#include <QDebug>

RepeatedProtoItem::RepeatedProtoItem(ProtobufModel& model, const proto::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : ProtoTreeItem(model, field, parentItem)
{
    init();
}

QItemDelegate *RepeatedProtoItem::getDelegate() const
{
    return nullptr;
}

QString RepeatedProtoItem::name() const
{
    return QString("%1 [%2]").arg(ProtoTreeItem::name()).arg(children().size());
}

void RepeatedProtoItem::fillFieldValue(google::protobuf::Message *message)
{
    for(auto& child: children())
        child->addFieldValue(message, field());
}


void RepeatedProtoItem::initFieldValue(const google::protobuf::Message * message)
{
    clearChildren();
    for(int i = 0, size = message->GetReflection()->FieldSize(*message, field()); i < size; ++i)
        createNode(field())->initRepeatedFieldValue(message, i);
}

void RepeatedProtoItem::init()
{
    QAction * act = new QAction(this);
    act->setText("Добавить");
    connect(act, &QAction::triggered, this, [&]() {
        model().beginAddItem(this);
        createNode(field());
        expand();
        model().endAddItem(parentItem());
    });
    addAction(act);
}

void RepeatedProtoItem::clearValue()
{
    clearChildren();
}
