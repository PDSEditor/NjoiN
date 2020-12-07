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
    QString oldPwd = ui->lineEdit->text();
    QString newPwd = ui->lineEdit_2->text();
    QString newPwd2 = ui->lineEdit_3->text();

    if (oldPwd.size() == 0 ){
        QMessageBox::warning(this, "Password", "Password not valid");
    }else if(QString::compare(newPwd, newPwd2, Qt::CaseSensitive) || newPwd.size()==0 || newPwd2.size()==0){
        QMessageBox::warning(this, "Password error", "Password non uguali o invalide");
    } else {
        this->setNewPwd(newPwd);
        this->setOldPwd(oldPwd);
        this->hide();
    }    
}

void ChangePwd::on_buttonBox_rejected()
{
    this->close();
}

void ChangePwd::setNewPwd(const QString &value)
{
    newPwd = value;
}

void ChangePwd::setOldPwd(const QString &value)
{
    oldPwd = value;
}

QString ChangePwd::getNewPassword(){
    return this->newPwd;
}

QString ChangePwd::getOldPassword(){
    return this->oldPwd;
}

