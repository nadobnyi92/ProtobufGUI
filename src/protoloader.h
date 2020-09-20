#ifndef PROTOLOADER_H
#define PROTOLOADER_H

#include <memory>

#include <QString>
#include <QHash>
#include <QUrl>

class ProtoContext;

namespace google
{
    namespace protobuf
    {
        class Descriptor;
    }
}

namespace proto = google::protobuf;

class ProtoLoader
{
public:
    ProtoLoader() = default;
    ProtoLoader(const ProtoLoader&) = delete;
    ProtoLoader& operator = (const ProtoLoader&) = delete;
    ~ProtoLoader();

    void load(const QUrl& path);

    QStringList protoPackages() const;
    QStringList protoClasses(const QString& package) const;
    const proto::Descriptor* proto(const QString& pPackage, const QString& pClass) const;

private:
    ProtoContext* mContext;
    QHash< QString, QHash<QString, const proto::Descriptor* > > mDescriptors;
};

#endif // PROTOLOADER_H
