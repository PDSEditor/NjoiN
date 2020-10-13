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
    if(ui->lineEdit_2->text()!=ui->lineEdit_3->text()){
        QMessageBox::information(this,"Errore password","Le due password non coincidono");
    }else if(ui->label->text().contains('_')){
        QMessageBox::information(this,"Errore username","Carattere '_' proibito");
    }else{
        Message m;
        m.setAction('E');
        m.setParams({ui->label->text(),ui->label_2->text()});
        m.setSender(sId);
        emit(sendMessage(&m));
    }
}

void Registration::openRw(int s)
{
    sId=s;
    this->exec();
}
