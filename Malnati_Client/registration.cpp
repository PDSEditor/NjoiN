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

void Registration::setSiteId(int s)
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
    auto username = ui->lineEdit->text();
    auto password = ui->lineEdit_2->text();
    auto password2 = ui->lineEdit_3->text();

    if(username.contains('_')){
        emit(sendError("username_"));
    }else if(username.size() == 0){
        emit(sendError("usernameNull"));
    }else if(QString::compare(password, password2, Qt::CaseSensitive) || password.size()==0 || password2.size() == 0){
        emit(sendError("password"));
    }else if(this->img.size() == 0){
        emit(sendError("immagine"));
    }else{
        Message m;
        m.setAction('E');
        m.setParams({ui->lineEdit->text(),ui->lineEdit_2->text(), img});
        m.setSender(sId);
        emit(sendMessage(&m));
    }
}

void Registration::on_pushButton_2_clicked()
{
    closeRw();
}

void Registration::on_pushButton_3_clicked()
{
    QFileDialog file_selection(ui->pushButton_3);
    file_selection.setNameFilter(tr("JPEG (*.jpg *.jpeg *.png)"));

    file_selection.show();

    QString filePath;
    if (file_selection.exec())
         filePath = file_selection.selectedFiles().at(0);

    QPixmap pix(filePath);
    QFile img(filePath);

    if (img.size() == 0) {
        // Don't send message
        return;
    } else if (img.size() > 2000000){
        QMessageBox::warning(this, "Warning", "The file's dimension is greater than 2MB!");
        return;
    }

    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    pix.save(&buffer, "PNG");
    auto const encoded = buffer.data().toBase64();
    this->img = encoded;

//    emit uploadImage(pix);
}

