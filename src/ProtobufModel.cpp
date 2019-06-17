#include "ProtobufModel.h"

ProtobufModel::ProtobufModel() :
    mProtoClass(nullptr)
{
    mRoleMapping[ProtoNameRole] = "name";
    mRoleMapping[ProtoTypeRole] = "type";
    mRoleMapping[ProtoRequirementsRole] = "requirment";
    mRoleMapping[ProtoValueRole] = "value";
}

void ProtobufModel::setProtoClass(const google::protobuf::Descriptor *protoclass)
{
    mProtoClass = protoclass;

}

QModelIndex ProtobufModel::index(int row, int column, const QModelIndex &parent) const
{
    return QModelIndex();
}

QModelIndex ProtobufModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int ProtobufModel::rowCount(const QModelIndex &parent) const
{
    return mProtoClass ? mProtoClass->field_count() : 0;
}

int ProtobufModel::columnCount(const QModelIndex &parent) const
{
    return mRoleMapping.size();
}

QVariant ProtobufModel::data(const QModelIndex &index, int role) const
{
    if(role == ProtoNameRole)
    {
        return QString::fromStdString(mProtoClass->field(index.row())->name());
    }
    return QVariant();
}

QHash<int, QByteArray> ProtobufModel::roleNames() const
{
    return mRoleMapping;
}
