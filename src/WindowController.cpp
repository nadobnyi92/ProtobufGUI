#include <iterator>
#include <iostream>

#include <QDir>
#include <QDirIterator>
#include <QUrl>
#include <QSet>
#include <QDebug>

#include "WindowController.h"

QString getFilePath(QDir root, QString filePath)
{
    return root.dirName() + "/" + root.relativeFilePath(filePath);
}

void WindowController::ErrorLog::AddError(const std::string & filename, int line,
              int column, const std::string & message)
{
    std::cout << "[" << filename << ":" << line << "]" << message << std::endl;
}

WindowController::WindowController(QObject *parent)
    : QObject(parent)
    , mProtoTreeModel(new ProtobufModel())
    , mImporter(&mProtoTree, &mError) {}

const QStringList& WindowController::getPackages() const { return mPackages; }
const QStringList& WindowController::getClasses() const { return mClasses; }
const QString& WindowController::getCurPackage() const { return mCurPackage; }
const QString& WindowController::getCurClass() const { return mCurClass; }
QAbstractItemModel * WindowController::getProtoTreeModel() const { return mProtoTreeModel; }

void WindowController::setCurPackage(const QString& packageName)
{
    if(mCurPackage == packageName)
        return;
    mCurPackage = packageName;
    mClasses.clear();
    for(auto const& data: mProtoPackages)
    {
        if(data.isPackage(mCurPackage))
            mClasses.append(data.className);
    }
    emit classesChange();
}

void WindowController::setCurClass(const QString& className)
{
    if(mCurClass == className)
        return;
    mCurClass = className;

    auto it = std::find_if(mProtoPackages.begin(), mProtoPackages.end(),
        [this](const ProtobufData& p) {return p.packageName == mCurPackage && p.className == mCurClass;});
    if(it != mProtoPackages.end())
    {
        mProtoTreeModel->setProtoClass(it->fDesc);
        emit protoTreeModelChange();
    }
}

void WindowController::loadProtoClasses(const QUrl& p)
{
    load(p.toLocalFile());
    mPackages.clear();
    for(auto const& data: mProtoPackages)
    {
        if(!mPackages.contains(data.packageName))
            mPackages.append(data.packageName);
    }
    emit packagesChange();
}

void WindowController::load(const QString& p)
{
    mProtoTree.MapPath("", p.toStdString());
    QDir root(p);

    QDirIterator it(p, QStringList() << "*.proto", QDir::Files, QDirIterator::Subdirectories);
    while(it.hasNext())
    {
        QString protoFile = it.next();
        const proto::FileDescriptor * fDesc = mImporter.Import( root.relativeFilePath(protoFile).toStdString());
        if(fDesc == nullptr)
            continue;

        for (int i = 0, c = fDesc->message_type_count(); i < c; ++i)
        {
            proto::Descriptor const * desc = fDesc->message_type(i);
            mProtoPackages.append(ProtobufData(fDesc->package().c_str(), desc->name().c_str(), desc));
        }
    }
}
