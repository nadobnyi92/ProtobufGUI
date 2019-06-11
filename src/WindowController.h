#ifndef WINDOWCOPNTROLLER_H
#define WINDOWCOPNTROLLER_H

#include <QObject>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/compiler/importer.h>

namespace protoc = google::protobuf::compiler;
namespace proto = google::protobuf;

class WindowController : public QObject
{
    class ErrorLog : public protoc::MultiFileErrorCollector
    {
    public:
        void AddError(const std::string & filename, int line,
                      int column, const std::string & message);
    };

    struct ProtobufData
    {
        ProtobufData() = default;
        ProtobufData(const QString& pName, const QString& cName, proto::Descriptor const * desc)
            : packageName(pName), className(cName), fDesc(desc) {}
        ProtobufData(const char * pName, const char * cName, proto::Descriptor const * desc)
            : packageName(pName), className(cName), fDesc(desc) {}

        QString packageName;
        QString className;
        proto::Descriptor const * fDesc;

        bool operator = (const ProtobufData& d) const
        {
            return  this->packageName == d.packageName &&
                    this->className  == d.className;
        }

        bool isPackage(const QString& packageName) const
        {
            return this->packageName == packageName;
        }
    };

    Q_OBJECT
    Q_PROPERTY(QString curPackage READ getCurPackage WRITE setCurPackage NOTIFY curPackageChange)
    Q_PROPERTY(QString curClass READ getCurClass WRITE setCurClass NOTIFY curClassChange)
    Q_PROPERTY(QStringList packages READ getPackages NOTIFY packagesChange)
    Q_PROPERTY(QStringList classes READ getClasses NOTIFY classesChange)

public:
    explicit WindowController(QObject *parent = nullptr)
        : QObject(parent), mImporter(&mProtoTree, &mError) { }

    const QStringList& getPackages() const;
    const QStringList& getClasses() const;

    const QString& getCurPackage() const;
    const QString& getCurClass() const;

    void setCurPackage(const QString& packageName);
    void setCurClass(const QString& className);

public slots:
    void loadProtoClasses(const QUrl& p);

signals:
    void curPackageChange();
    void curClassChange();
    void packagesChange();
    void classesChange();

private:
    QList<ProtobufData> mProtoPackages;

    QStringList mPackages;
    QStringList mClasses;

    QString mCurPackage;
    QString mCurClass;

    protoc::DiskSourceTree mProtoTree;
    protoc::Importer mImporter;
    proto::DynamicMessageFactory mFactory;
    ErrorLog mError;

    void load(const QString &p);
    void initTree();
};

#endif // WINDOWCOPNTROLLER_H
