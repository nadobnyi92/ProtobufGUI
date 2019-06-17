#ifndef PROTOBUFMODEL_H
#define PROTOBUFMODEL_H

#include <QAbstractItemModel>

#include <google/protobuf/dynamic_message.h>

namespace proto = google::protobuf;

class ProtobufModel : public QAbstractItemModel
{
public:
    enum ProtobufModelRoles
    {
        ProtoNameRole = Qt::UserRole + 1,
        ProtoTypeRole,
        ProtoRequirementsRole,
        ProtoValueRole
    };

    ProtobufModel();

    void setProtoClass(proto::Descriptor const * protoclass);

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QHash<int, QByteArray> mRoleMapping;
    proto::Descriptor const * mProtoClass;
};

#endif // PROTOBUFMODEL_H
