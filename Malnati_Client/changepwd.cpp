#include "changepwd.h"
#include "ui_changepwd.h"
#include <QMessageBox>

ChangePwd::ChangePwd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePwd)
{
    ui->setupUi(this);
    setWindowTitle("Change Password");
}

ChangePwd::~ChangePwd()
{
    delete ui;
}

void ChangePwd::on_buttonBox_accepted()
{
    QString pwd = ui->lineEdit->text();
    if ( pwd.size() == 0 ){
        QMessageBox::warning(this, "Password", "Password not valid");
    } else {
        emit pwdUpdate(pwd);
        this->hide();
    }    
}

void ChangePwd::on_buttonBox_rejected()
{
    this->hide();
}
