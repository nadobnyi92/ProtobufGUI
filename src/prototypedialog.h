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

public slots:
    void onSetPackages(const QStringList&);
    void onSetClasses(const QStringList&);

private:
    Ui::ProtoTypeDialog * ui;
    const ProtoManager& mManger;

};

#endif // PROTOTYPEDIALOG_H
