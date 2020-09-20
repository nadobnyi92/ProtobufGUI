#ifndef PROTOMANAGER_H
#define PROTOMANAGER_H

#include <memory>

#include <QObject>
#include <QHash>

#include "protoloader.h"
#include "tree_items/prototreeitem.h"

QStringList removeEmptyOrDupl(const QStringList& src);

class ProtoManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList protoPackages READ protoPackages NOTIFY protoPackagesChanged)
    Q_PROPERTY(QStringList protoClasses READ protoClasses NOTIFY protoClassesChanged)
    Q_PROPERTY(QString curPackage READ curPackage WRITE setCurPackage NOTIFY curPackageChanged)
    Q_PROPERTY(QString curClass READ curClass WRITE setCurClass NOTIFY curClassChanged)
    Q_PROPERTY(ProtoTreeItem* rootItem READ rootItem NOTIFY rootItemChanged)

public:
    explicit ProtoManager(QObject *parent = nullptr);

    Q_INVOKABLE void load(const QUrl &path);
    Q_INVOKABLE ProtoManager* clone();

    QStringList protoPackages() const;
    QStringList protoClasses() const;
    const QString& curPackage() const;
    const QString& curClass() const;
    ProtoTreeItem* rootItem() const;

    void setCurPackage(const QString&);
    void setCurClass(const QString&);

signals:
    void protoPackagesChanged(const QStringList& packages);
    void protoClassesChanged(const QStringList& classes);
    void curPackageChanged(const QString& curPackage);
    void curClassChanged(const QString& curClass);
    void rootItemChanged(const ProtoTreeItem& item);

private:
    explicit ProtoManager(const std::shared_ptr<ProtoLoader>& loader, QObject * parent = nullptr);

private:
    std::shared_ptr<ProtoLoader> mLoader;
    std::unique_ptr<ProtoTreeItem> mRootITem;

    QString mCurPackage;
    QString mCurClass;
};

#endif // PROTOMANAGER_H
