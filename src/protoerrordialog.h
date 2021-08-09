#ifndef PROTOERRORDIALOG_H
#define PROTOERRORDIALOG_H

#include <QDialog>
#include <QObject>

#include "prototreeerror.h"

namespace Ui { class ProtoErrorDialog; }

class ProtoErrorDialog : public QDialog
{
    Q_OBJECT
public:
    ProtoErrorDialog(QWidget * parent = nullptr);

    void addError(const ProtoMessageError& err);
    void addErrors(const QList<ProtoMessageError>& errs);

private:
    QStringList errFiles();
    QStringList errMessages(const QString& file);
    void displayErrors(const QString& filename);

private:
    Ui::ProtoErrorDialog *ui;
    QList<ProtoMessageError> mErrors;
};

#endif // PROTOERRORDIALOG_H
