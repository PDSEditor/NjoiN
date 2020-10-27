#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>
#include <QStyle>
#include "message.h"
#include <QApplication>
#include <QDesktopWidget>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    isLogin = false;
    ui->setupUi(this);

    this->ui->logo1->setStyleSheet("background-image: url(:/images/Icons/logo-icon.png);background-repeat:none;background-position:center;color:red");
    setWindowTitle("Login");
}


LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::closeEvent(QCloseEvent *e){
}

void LoginWindow::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if ( username.size() == 0 || password.size() == 0 ) {
            QMessageBox::warning(this, "Errore login", "Username e password non possono essere vuoti");
    } else {
        Message m;
        m.setAction('L');
        m.setParams({username,password});
        m.setSender(siteId);
        emit(sendMessage(&m));
    }

}

void LoginWindow::receivedLogin(bool resp){

    if(resp){
        isLogin=true;
            QMessageBox::information(this,"Login success","Username e password corretti");
            setIsLogin(true);
            this->accept();
    }
    else {
        QMessageBox::critical(this,"Errore login","Username e password non sono corretti");
        qDebug() << "errore login";
        isLogin=false;
    }

}

void LoginWindow::receivedSiteId(int siteid){
    siteId=siteid;
}

void LoginWindow::receiveRegistration(Message m)
{
    if(m.getError()){
        QMessageBox::warning(this,"Errore","Username già presente, utilizzarne un altro");
        emit(openRw(siteId));
    }
    else{
        QMessageBox::information(this,"Registrazione avvenuta","La registrazione è avvenuta correttamente.\nEsegui il login!");
        emit(closeRw());
    }
}

void LoginWindow::loggedin(bool c)
{
    if(c)
        QMessageBox::warning(this,"Errore login","Utente già loggato");
    else
        QMessageBox::critical(this,"ERRORE","Impossibile connettersi al server");
}

void LoginWindow::receiveErrorReg(QString s)
{
    if(QString::compare(s, "password", Qt::CaseSensitive)==0){
        QMessageBox::critical(this,"Errore password","Le due password non coincidono");
        emit(openRw(siteId));
    }else if(QString::compare(s, "username_", Qt::CaseSensitive)==0){
        QMessageBox::critical(this,"Errore username","Carattere '_' proibito");
        emit(openRw(siteId));
    }else if(QString::compare(s, "usernameNull", Qt::CaseSensitive)==0){
        QMessageBox::critical(this,"Errore username","Inserisci un username valido");
        emit(openRw(siteId));
    }else if(QString::compare(s, "immagine", Qt::CaseSensitive)==0){
        QMessageBox::critical(this, "Errore immagine", "Caricare un'immagine valida");
        emit(openRw(siteId));
    }
}

bool LoginWindow::getIsLogin() const{
    return isLogin;
}

void LoginWindow::setIsLogin(bool isLogin){
   this->isLogin = isLogin;
}

void LoginWindow::on_pushButton_signup_clicked(){
    emit(openRw(siteId));
}


