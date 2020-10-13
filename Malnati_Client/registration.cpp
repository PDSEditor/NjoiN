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

void Registration::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->lineEdit_2->text()!=ui->lineEdit_3->text()||ui->lineEdit_2->text().isEmpty()||ui->lineEdit_3->text().isEmpty()){
        emit(sendError("password"));
    }else if(ui->label->text().contains('_')){
        emit(sendError("username"));
    }else{
        Message m;
        m.setAction('E');
        m.setParams({ui->lineEdit->text(),ui->lineEdit_2->text()});
        m.setSender(sId);
        emit(sendMessage(&m));
    }
}

void Registration::openRw(int s)
{
    sId=s;
    this->setWindowModality(Qt::WindowModal);
    this->show();
}

void Registration::closeRw()
{
    this->close();
}
