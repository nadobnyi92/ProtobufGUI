#include "protoerrordialog.h"
#include "ui_protoerrordialog.h"

ProtoErrorDialog::ProtoErrorDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::ProtoErrorDialog)
{
    ui->setupUi(this);

    connect(ui->cbFileName, &QComboBox::currentTextChanged,
            this, &ProtoErrorDialog::displayErrors);
}

void ProtoErrorDialog::addError(const ProtoMessageError &err)
{
    mErrors.append(err);
    ui->cbFileName->clear();
    ui->cbFileName->addItems(errFiles());
}

void ProtoErrorDialog::addErrors(const QList<ProtoMessageError> &errs)
{
    for(auto err: errs) {
        mErrors.append(err);
    }
    ui->cbFileName->clear();
    ui->cbFileName->addItems(errFiles());
}

QStringList ProtoErrorDialog::errFiles()
{
    QSet<QString> files;
    for(auto err: mErrors) { files.insert(err.filename); }
    return QStringList(files.begin(), files.end());
}

QStringList ProtoErrorDialog::errMessages(const QString &filename)
{
    QStringList messages;
    for(auto err: mErrors) {
        if(err.filename == filename) {
            messages.append(QString("%1: %2")
                            .arg(err.line)
                            .arg(err.message));
        }
    }
    return messages;
}

void ProtoErrorDialog::displayErrors(const QString &filename)
{
    ui->lwErrors->clear();
    ui->lwErrors->addItems(errMessages(filename));
}
