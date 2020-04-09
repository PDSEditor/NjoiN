#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>
#include <QStyle>
#include "message.h"
#include <QApplication>
#include <QDesktopWidget>

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    isLogin = 0;
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



    if(username == "test" && password == "test"){
        QMessageBox::information(this,"Login","Username e password corretti");
        mw = new MainWindow(this);

       // TextEdit te;

        const QRect availableGeometryMw = QApplication::desktop()->availableGeometry(mw);
      //  mw->resize(availableGeometryMw.width() / 2, (availableGeometryMw.height() * 2) / 3);
      /*  mw->move((availableGeometryMw.width() - mw->width()) / 2,
                (availableGeometryMw.height() - mw->height()) / 2);*/

        int width = availableGeometryMw.width();
            int height = availableGeometryMw.height();
            width *= 0.9; // 90% of the screen size
            height *= 0.9; // 90% of the screen size
            QSize newSize( width, height );
            setGeometry(
                   QStyle::alignedRect(
                       Qt::LeftToRight,
                       Qt::AlignCenter,
                       newSize,
                       QApplication::desktop()->availableGeometry(mw)
                   )
               );


        //APRE UNA PAGINA DI PRESENTAZIONE DEL TEXTEDIT
       // if (!mw.load(parser.positionalArguments().value(0, QLatin1String(":/example.html"))))

        //te.fileNew();
        mw->show();
        hide();
    }
    else{
        QMessageBox::warning(this,"Login","Username e password non sono corretti");
        isLogin=0;
    }
}

int LoginWindow::getIsLogin() const{
    return isLogin;
}
void LoginWindow::setIsLogin(int isLogin){
   this->isLogin = isLogin;
}

