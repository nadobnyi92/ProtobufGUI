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
    return root.dirName() + QDir::separator() + root.relativeFilePath(filePath);
}

void WindowController::ErrorLog::AddError(const std::string & filename, int line,
              int column, const std::string & message)
{
    std::cout << "[" << filename << ":" << line << "]" << message << std::endl;
}

const QStringList& WindowController::getPackages() const { return mPackages; }
const QStringList& WindowController::getClasses() const { return mClasses; }
const QString& WindowController::getCurPackage() const { return mCurPackage; }
const QString& WindowController::getCurClass() const { return mCurClass; }

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
    initTree();
    //emit curClassChange();
}

void WindowController::loadProtoClasses(const QUrl& p)
{
    load(p.path());
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
        const proto::FileDescriptor * fDesc = mImporter.Import(getFilePath(root, protoFile).toStdString());
        if(fDesc == nullptr)
            continue;

        for (int i = 0, c = fDesc->message_type_count(); i < c; ++i)
        {
            proto::Descriptor const * desc = fDesc->message_type(i);
            mProtoPackages.append(ProtobufData(fDesc->package().c_str(), desc->name().c_str(), desc));
        }
    }
}

void WindowController::initTree()
{

}
