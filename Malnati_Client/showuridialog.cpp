#include "showuridialog.h"
#include "ui_showuridialog.h"
#include <QCloseEvent>

ShowUriDialog::ShowUriDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowUriDialog)
{
    ui->setupUi(this);
    b= new QSignalBlocker(this);

}

void ShowUriDialog::setUriDialog(QString uri)
{
    ui->UriLabel->setText(uri);
}

void ShowUriDialog::closeEvent(QCloseEvent *event){

    this->close();


}

ShowUriDialog::~ShowUriDialog()
{
    delete ui;
}

