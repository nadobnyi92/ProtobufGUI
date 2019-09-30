#ifndef PROTOTYPEDIALOG_H
#define PROTOTYPEDIALOG_H

#include <QDialog>

#include "protomanager.h"

namespace Ui
{
    class ProtoTypeDialog;
}

class ProtoTypeDialog : public QDialog
{
    Q_OBJECT
public:
    ProtoTypeDialog(const ProtoManager& manager, QWidget * parent = nullptr);

    QString pPackage() const;
    QString pClass() const;

public slots:
    void onSetPackage(const QString& package);

private:
    Ui::ProtoTypeDialog * ui;

    QMultiHash<QString, QString> mClasses;

};

#endif // PROTOTYPEDIALOG_H
