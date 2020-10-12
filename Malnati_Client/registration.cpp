#include "registration.h"
#include "ui_registration.h"

Registration::Registration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);
}

Registration::Registration(int siteId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);
    sId=siteId;
}

Registration::~Registration()
{
    delete ui;
}

void Registration::receiveRegistration(Message m)
{
    if(m.getError()){
        QMessageBox::information(this,"Errore","Username già presente, utilizzarne un altro");
    }
    else{
        QMessageBox::information(this,"Registrazione avvenuta","La registrazione è avvenuta correttamente");
    }
}

void Registration::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->label_2->text()!=ui->label_3->text()){
        QMessageBox::information(this,"Errore password","Le due password non coincidono");
    }else if(ui->label->text().contains('_')){
        QMessageBox::information(this,"Errore username","Carattere '_' proibito");
    }else{
        Message m;
        m.setAction('E');
        m.setParams({ui->label->text(),ui->label_2->text()});
        m.setSender(sId);
        emit(sendRegistration(m));
    }
}
