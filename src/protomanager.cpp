#include <QDir>
#include <QDirIterator>
#include <QMessageBox>
#include <QSet>
#include <QDebug>

#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>

#include "protomanager.h"
#include "prototreeerror.h"
#include "protoerrordialog.h"

namespace protoc = google::protobuf::compiler;
namespace proto = google::protobuf;

QStringList removeEmptyOrDupl(const QStringList& src)
{
    QStringList res;
    std::copy_if(src.begin(), src.end(),
                 std::back_inserter(res),
                 [&](const QString& val) {return !res.contains(val) && !val.trimmed().isEmpty();});
    res.sort();
    return res;
}

QStringList removeDupl(const QStringList& src)
{
    QStringList res;
    std::copy_if(src.begin(), src.end(),
                 std::back_inserter(res),
                 [&](const QString& val) {return !res.contains(val) && !val.trimmed().isEmpty();});
    res.sort();
    return res;
}

class ProtoContext
{
private:
    class ErrorLog : public protoc::MultiFileErrorCollector
    {
    public:
        void AddError(const std::string & filename, int line,
                      int /*column*/, const std::string & message)
        {
            std::cout << "[" << filename << ":" << line << "]" << message << std::endl;
            QString qFileName = QString::fromStdString(filename);
            QString qMessage = QString::fromStdString(message);
            mErrors.append(ProtoMessageError {qFileName, line, qMessage});
        }

        const QList<ProtoMessageError>& errors() const { return mErrors; }

    private:
        QList<ProtoMessageError> mErrors;
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

    const QList<ProtoMessageError>& errors() const { return mError.errors(); }

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
    : QObject(parent) {}

ProtoManager::~ProtoManager() {}

void ProtoManager::setPackage(const QString& pPackage)
{
    mCurPackage = pPackage;
    emit onPackageChange(removeDupl(mProtoPackages[mCurPackage].keys()));
}

void ProtoManager::setClass(const QString& pClass)
{
    mCurClass = pClass;
    emit onClassChange(mProtoPackages
        .value(mCurPackage, QHash<QString, const proto::Descriptor*>())
        .value(mCurClass, nullptr));
}

void ProtoManager::load(const QUrl& path)
{
    mProtoPackages.clear();
    mProtoPath = path;
    reload();
}

void ProtoManager::reload()
{
    QFileInfo info { mProtoPath.toLocalFile() };
    info.isDir() ? reloadDir() : reloadFile();
}

void ProtoManager::reloadDir()
{
    QString p = mProtoPath.toLocalFile();
    context.reset(new ProtoContext());
    context->mapPath("", p.toStdString());
    QDir root(p);

    QDirIterator it(p, QStringList() << "*.proto", QDir::Files, QDirIterator::Subdirectories);
    while(it.hasNext()) {
        QString protoFile = it.next();
        const proto::FileDescriptor * fDesc = context->import(root.relativeFilePath(protoFile).toStdString());
        if(fDesc == nullptr)
            continue;

        for (int i = 0, c = fDesc->message_type_count(); i < c; ++i) {
            proto::Descriptor const * desc = fDesc->message_type(i);
            std::cout << fDesc->package().c_str() << std::endl;
            mProtoPackages[fDesc->package().c_str()][desc->name().c_str()] = desc;
        }
    }
    if(context->errors().size() > 0) {
        ProtoErrorDialog dlg;
        dlg.addErrors(context->errors());
        dlg.exec();
    }
    emit onProtoChange(removeEmptyOrDupl(mProtoPackages.keys()));
}

void ProtoManager::reloadFile()
{
    QFileInfo info { mProtoPath.toLocalFile() };
    QDir root = info.absoluteDir();
    qDebug() << root.absolutePath();
    context.reset(new ProtoContext());
    context->mapPath("", root.absolutePath().toStdString());

    qDebug() << root.relativeFilePath( info.absoluteFilePath() );
    const proto::FileDescriptor * fDesc = context->import(root.relativeFilePath( info.absoluteFilePath() ).toStdString());
    if(fDesc == nullptr)
        return;

    for (int i = 0, c = fDesc->message_type_count(); i < c; ++i) {
        proto::Descriptor const * desc = fDesc->message_type(i);
        std::cout << fDesc->package().c_str() << std::endl;
        mProtoPackages[fDesc->package().c_str()][desc->name().c_str()] = desc;
    }
    if(context->errors().size() > 0) {
        ProtoErrorDialog dlg;
        dlg.addErrors(context->errors());
        dlg.exec();
    }
    emit onProtoChange(removeEmptyOrDupl(mProtoPackages.keys()));
}


QMultiHash<QString, QString> ProtoManager::getProtoClasses() const
{
    QMultiHash<QString, QString> protoClasses;
    for(auto itPkg = mProtoPackages.begin(), itPkg_end = mProtoPackages.end();
        itPkg != itPkg_end; ++itPkg) {
        for(auto itCls = itPkg.value().begin(), itCls_end = itPkg.value().end();
            itCls != itCls_end; ++itCls) {
            protoClasses.insert(itPkg.key(), itCls.key());
        }
    }
    return protoClasses;
}

const proto::Descriptor *ProtoManager::getClassDescriptor(const QString &pPackage, const QString &pClass) const
{
    return mProtoPackages
        .value(pPackage, QHash<QString, const proto::Descriptor*>())
        .value(pClass, nullptr);
}
