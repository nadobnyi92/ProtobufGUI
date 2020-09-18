#include "protoloader.h"

#include <QDirIterator>

#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>

namespace protoc = google::protobuf::compiler;
namespace proto = google::protobuf;

class ProtoContext
{
private:
    class ErrorLog : public protoc::MultiFileErrorCollector
    {
    public:
        void AddError(const std::string & filename, int line,
                      int column, const std::string & message)
        {
            std::cout << "[" << filename << ":" << line << "]" << message << std::endl;
        }
    };

public:
    ProtoContext() : mImporter(&mProtoTree, &mError) {}

    const proto::FileDescriptor * import(const std::string& path)
    {
        return mImporter.Import(path);
    }

    void mapPath(const std::string& virtualPath, const std::string& diskPath)
    {
        mProtoTree.MapPath(virtualPath, diskPath);
    }

private:
    protoc::DiskSourceTree mProtoTree;
    protoc::Importer mImporter;
    proto::DynamicMessageFactory mFactory;

    ErrorLog mError;
};

ProtoLoader::~ProtoLoader()
{
    if(mContext)
        delete mContext;
}

void ProtoLoader::load(const QUrl &path)
{
    mDescriptors.clear();

    QString p = path.toLocalFile();
    if(mContext) delete mContext;
    mContext = new ProtoContext();
    mContext->mapPath("", p.toStdString());
    QDir root(p);

    QDirIterator it(p, QStringList() << "*.proto", QDir::Files, QDirIterator::Subdirectories);
    while(it.hasNext()) {
        QString protoFile = it.next();
        const proto::FileDescriptor * fDesc = mContext->import(root.relativeFilePath(protoFile).toStdString());
        if(fDesc == nullptr)
            continue;

        for (int i = 0, c = fDesc->message_type_count(); i < c; ++i) {
            proto::Descriptor const * desc = fDesc->message_type(i);
            mDescriptors[fDesc->package().c_str()][desc->name().c_str()] = desc;
        }
    }
}

QStringList ProtoLoader::protoPackages() const
{
    return mDescriptors.keys();
}

QStringList ProtoLoader::protoClasses(const QString &package) const
{
    return mDescriptors[package].keys();
}
