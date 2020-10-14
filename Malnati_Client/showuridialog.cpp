#include "showuridialog.h"
#include "ui_showuridialog.h"
#include <QCloseEvent>

ShowUriDialog::ShowUriDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowUriDialog)
{
    ui->setupUi(this);

}

void ShowUriDialog::setUriDialog(QString uri)
{
    ui->UriLabel->setText(uri);
}



ShowUriDialog::~ShowUriDialog()
{
    delete ui;
}

void ShowUriDialog::closeEvent(QCloseEvent *e)
{
    e->Hide;
}
