#include <iterator>

#include <QSet>

#include "WindowController.h"

const QStringList& WindowController::getPackages() const { return mPackages; }
const QStringList& WindowController::getClasses() const { return mClasses; }
const QString& WindowController::getCurPackage() const { return mCurPackage; }
const QString& WindowController::getCurClass() const { return mCurClass; }

void WindowController::setCurPackage(const QString& packageName)
{
    if(mCurPackage == packageName)
        return;
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
    //emit curClassChange();
}

void WindowController::loadProtoClasses(const QString& path)
{
    load(path);
    mPackages.clear();
    for(auto const& data: mProtoPackages)
    {
        if(!mPackages.contains(data.packageName))
            mPackages.append(data.packageName);
    }
    emit packagesChange();
}

void WindowController::load(const QString& path)
{
    mProtoPackages = QList<ProtobufData>() <<
            ProtobufData("package1", "class11") <<
            ProtobufData("package1", "class12") <<
            ProtobufData("package1", "class13") <<
            ProtobufData("package2", "class21") <<
            ProtobufData("package2", "class22") <<
            ProtobufData("package2", "class23") <<
            ProtobufData("package2", "class24") <<
            ProtobufData("package1", "class31") <<
            ProtobufData("package1", "class32");
//todo load from protobuf
}

