#include "showuridialog.h"
#include "ui_showuridialog.h"

ShowUriDialog::ShowUriDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowUriDialog)
{
    ui->setupUi(this);

}

ShowUriDialog::ShowUriDialog(QWidget *parent, QString uri):
    QDialog(parent),
    ui(new Ui::ShowUriDialog)
{
    ui->setupUi(this);
    ui->UriLabel->setText(uri);
}

ShowUriDialog::~ShowUriDialog()
{
    delete ui;
}
