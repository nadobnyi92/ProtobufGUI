#include <iterator>
#include <iostream>

#include <QDir>
#include <QDirIterator>
#include <QUrl>
#include <QSet>
#include <QDebug>

#include "WindowController.h"

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
    //emit curClassChange();
}

void WindowController::loadProtoClasses(const QUrl& path)
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

void WindowController::load(const QUrl& path)
{
    std::cout << "load proto classes from " << path.path().toStdString() << std::endl;

    QDirIterator it(path.path(), QStringList() << "*.proto", QDir::Files, QDirIterator::Subdirectories);
    while(it.hasNext())
    {
        std::cout << it.next().toStdString() << std::endl;
    }
}

