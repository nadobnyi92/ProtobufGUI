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

ProtoTreeItem* ProtoManager::rootItem() const
{
    return mRootITem.get();
}

void ProtoManager::setCurPackage(const QString & curPackage)
{
    mCurPackage = curPackage;
    emit protoClassesChanged(protoClasses());
}

void ProtoManager::setCurClass(const QString & curClass)
{
    mCurClass = curClass;
    mRootITem.reset(new ProtoTreeItem(mLoader->proto(mCurClass, mCurPackage)));
}
