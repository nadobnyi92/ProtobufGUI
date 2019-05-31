#ifndef WINDOWCOPNTROLLER_H
#define WINDOWCOPNTROLLER_H

#include <QObject>

class WindowController : public QObject
{
    struct ProtobufData
    {
        ProtobufData() = default;
        ProtobufData(const QString& pName, const QString& cName)
            : packageName(pName), className(cName) {}
        ProtobufData(const char * pName, const char * cName)
            : packageName(pName), className(cName) {}

        QString packageName;
        QString className;

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
        : QObject(parent) { }

    const QStringList& getPackages() const;
    const QStringList& getClasses() const;

    const QString& getCurPackage() const;
    const QString& getCurClass() const;

    void setCurPackage(const QString& packageName);
    void setCurClass(const QString& className);

public slots:
    void loadProtoClasses(const QString& path);

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

    void load(const QString &path);
};

#endif // WINDOWCOPNTROLLER_H
