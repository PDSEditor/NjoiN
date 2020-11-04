#include "showuridialog.h"
#include "ui_showuridialog.h"
#include <QCloseEvent>
#include "simplecrypt.h"

ShowUriDialog::ShowUriDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowUriDialog)
{
    ui->setupUi(this);
    b= new QSignalBlocker(this);

}

void ShowUriDialog::setUriDialog(QString uri)
{
    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023));
    QString enc = crypto.encryptToString(uri);
    ui->UriLabel->setText(enc);
}

void ShowUriDialog::closeEvent(QCloseEvent *event){
    this->close();
}

ShowUriDialog::~ShowUriDialog()
{
    delete ui;
}

