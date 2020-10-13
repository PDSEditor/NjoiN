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

    this->ui->logo1->setStyleSheet("background-image: url(:/images/Icons/logo-icon.png);background-repeat:none;background-position:center;");
    setWindowTitle("Login");


}


LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::closeEvent(QCloseEvent *e){
   exit;
}

void LoginWindow::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if ( username.size() == 0 || password.size() == 0 ) {
            QMessageBox::warning(this, "Login Error", "Username and password cannot be empty");
    } else {
        Message m;
        m.setAction('L');
        m.setParams({username,password});
        //da cancellare assolutamente!!!!
        m.setSender(siteId);
        //
        emit(sendMessage(&m));
        //qui bisogna attendere che il server ritorni il siteId
        //    if(isLogin){                               //sostituire con Islogin==1!!!!!!
        //        QMessageBox::information(this,"Login","Username e password corretti");
        //       // mw = new MainWindow(this);
        //        setIsLogin(true);
        //       // TextEdit te;
        //      //  mw->resize(availableGeometryMw.width() / 2, (availableGeometryMw.height() * 2) / 3);
        //      /*  mw->move((availableGeometryMw.width() - mw->width()) / 2,
        //                (availableGeometryMw.height() - mw->height()) / 2);*/


        //        this->accept();

        //        //APRE UNA PAGINA DI PRESENTAZIONE DEL TEXTEDIT
        //       // if (!mw.load(parser.positionalArguments().value(0, QLatin1String(":/example.html"))))

        //        //te.fileNew();
        //       // mw->show();
        //       // hide();
        //    }
        //    else{
        //        QMessageBox::warning(this,"Login","Username e password non sono corretti");
        //        isLogin=false;
        //    }
    }

}

void LoginWindow::receivedLogin(bool resp){

    if(resp){
        isLogin=true;                              //sostituire con Islogin==1!!!!!!
            QMessageBox::information(this,"Login","Username e password corretti");
           // mw = new MainWindow(this);
            setIsLogin(true);
           // TextEdit te;
          //  mw->resize(availableGeometryMw.width() / 2, (availableGeometryMw.height() * 2) / 3);
          /*  mw->move((availableGeometryMw.width() - mw->width()) / 2,
                    (availableGeometryMw.height() - mw->height()) / 2);*/


            this->accept();

            //APRE UNA PAGINA DI PRESENTAZIONE DEL TEXTEDIT
           // if (!mw.load(parser.positionalArguments().value(0, QLatin1String(":/example.html"))))

            //te.fileNew();
           // mw->show();
           // hide();
    }
    else {
        QMessageBox::information(this,"Login","Username e password non sono corretti");
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
        QMessageBox::information(this,"Errore","Username già presente, utilizzarne un altro");
        emit(openRw(siteId));
    }
    else{
        QMessageBox::information(this,"Registrazione avvenuta","La registrazione è avvenuta correttamente");
        emit(closeRw());
    }
}

void LoginWindow::loggedin(bool c)
{
    if(c==1)
        QMessageBox::information(this,"Login","Utente già loggato");
    else
        QMessageBox::information(this,"ERRORE","Impossibile connettersi al server");
}

void LoginWindow::receiveErrorReg(QString s)
{
    if(s=="password"){
        QMessageBox::information(this,"Errore password","Le due password non coincidono");
        emit(openRw(siteId));
    }else if(s=="usenrame"){
        QMessageBox::information(this,"Errore username","Carattere '_' proibito");
        emit(openRw(siteId));
    }
}




bool LoginWindow::getIsLogin() const{
    return isLogin;
}
void LoginWindow::setIsLogin(int isLogin){
   this->isLogin = isLogin;
}

void LoginWindow::on_pushButton_signup_clicked(){


    emit(openRw(siteId));





}


