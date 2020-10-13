#include "registration.h"
#include "ui_registration.h"

Registration::Registration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);
}


Registration::~Registration()
{
    delete ui;
}

void Registration::setiteId(int s)
{
    sId=s;
}

void Registration::receiveRegistration(Message m)
{

}


void Registration::openRw(int s)
{
    sId=s;
    this->setWindowModality(Qt::ApplicationModal);
    this->show();
}

void Registration::closeRw()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    this->close();
}


void Registration::on_pushButton_clicked()
{
    if(ui->lineEdit_2->text()!=ui->lineEdit_3->text()||ui->lineEdit_2->text().isNull()||ui->lineEdit_3->text().isNull()){
        emit(sendError("password"));
    }else if(ui->lineEdit->text().contains('_')){
        emit(sendError("username"));
    }else{
        Message m;
        m.setAction('E');
        m.setParams({ui->lineEdit->text(),ui->lineEdit_2->text()});
        m.setSender(sId);
        emit(sendMessage(&m));
    }
}

void Registration::on_pushButton_2_clicked()
{
    closeRw();
}
