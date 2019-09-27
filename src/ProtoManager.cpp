#include <QDir>
#include <QUrl>
#include <QDirIterator>

#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>

#include "ProtoManager.h"

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

QString getFilePath(QDir root, QString filePath)
{
    return root.dirName() + "/" + root.relativeFilePath(filePath);
}

ProtoManager::ProtoManager(QObject *parent)
    : QObject(parent), context(nullptr) {}

ProtoManager::~ProtoManager()
{
    if(context)
        delete context;
}

void ProtoManager::setPackage(const QString& pPackage)
{
    mCurPackage = pPackage;
    emit onPackageChange(mProtoPackages[mCurPackage].keys());
}

void ProtoManager::setClass(const QString& pClass)
{
    mCurClass = pClass;
    emit onClassChange(mProtoPackages[mCurPackage][mCurClass]);
}

void ProtoManager::load(const QUrl& path)
{
    if(context)
        delete context;
    mProtoPackages.clear();

    QString p = path.toLocalFile();

    context = new ProtoContext();
    context->mapPath("", p.toStdString());
    QDir root(p);

    QDirIterator it(p, QStringList() << "*.proto", QDir::Files, QDirIterator::Subdirectories);
    while(it.hasNext())
    {
        QString protoFile = it.next();
        const proto::FileDescriptor * fDesc = context->import(root.relativeFilePath(protoFile).toStdString());
        if(fDesc == nullptr)
            continue;

        for (int i = 0, c = fDesc->message_type_count(); i < c; ++i)
        {
            proto::Descriptor const * desc = fDesc->message_type(i);
            mProtoPackages[fDesc->package().c_str()][desc->name().c_str()] = desc;
        }
    }
    emit onProtoChange(mProtoPackages.keys());
}

QStringList ProtoManager::getPackages() const
{
    return mProtoPackages.keys();
}
