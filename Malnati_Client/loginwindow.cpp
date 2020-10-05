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
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    Message m;
    m.setAction('L');
    m.setParams({username,password});
    //da cancellare assolutamente!!!!
    m.setSender(0);
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

void LoginWindow::receivedLogin(bool resp){

    if(resp){
        isLogin=true;
        if(isLogin){                               //sostituire con Islogin==1!!!!!!
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
        else{
            QMessageBox::warning(this,"Login","Username e password non sono corretti");
            isLogin=false;
        }
    }
    else {
        QMessageBox::warning(this,"Login","Username e password non sono corretti");
        isLogin=false;
    }

}
bool LoginWindow::getIsLogin() const{
    return isLogin;
}
void LoginWindow::setIsLogin(int isLogin){
   this->isLogin = isLogin;
}

void LoginWindow::on_pushButton_signup_clicked(){
    qDebug() << "Registrazione";
    //todo: fare la registrationWindow





}

