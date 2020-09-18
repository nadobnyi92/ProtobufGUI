#include <QDir>

#include "protomanager.h"

QStringList removeEmptyOrDupl(const QStringList& src)
{
    QStringList res;
    std::copy_if(src.begin(), src.end(),
                 std::back_inserter(res),
                 [&](const QString& val) {return !res.contains(val) && !val.trimmed().isEmpty();});
    res.sort();
    return res;
}

QString getFilePath(QDir root, QString filePath)
{
    return root.dirName() + "/" + root.relativeFilePath(filePath);
}

ProtoManager::ProtoManager(QObject *parent)
    : QObject(parent)
    , mLoader(new ProtoLoader) {}

ProtoManager::ProtoManager(const std::shared_ptr<ProtoLoader>& loader, QObject * parent)
    : QObject(parent)
    , mLoader(loader) {}

void ProtoManager::load(const QUrl& path)
{
    mLoader->load(path);
    emit protoPackagesChanged(protoPackages());
}

ProtoManager *ProtoManager::clone()
{
    return new ProtoManager(mLoader, this);
}

QStringList ProtoManager::protoPackages() const
{
    return removeEmptyOrDupl(mLoader->protoPackages());
}

QStringList ProtoManager::protoClasses() const
{
    return removeEmptyOrDupl(mLoader->protoClasses(mCurPackage));
}

const QString &ProtoManager::curPackage() const
{
    return mCurPackage;
}

const QString &ProtoManager::curClass() const
{
    return mCurClass;
}

//const proto::Descriptor *ProtoManager::getClassDescriptor(const QString &pPackage, const QString &pClass) const
//{
//    return mProtoPackages
//        .value(pPackage, QHash<QString, const proto::Descriptor*>())
//            .value(pClass, nullptr);
//}

void ProtoManager::setCurPackage(const QString & curPackage)
{
    mCurPackage = curPackage;
    emit protoClassesChanged(protoClasses());
}

void ProtoManager::setCurClass(const QString & curClass)
{
    mCurClass = curClass;
}
