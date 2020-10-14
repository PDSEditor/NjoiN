#include "inserttitle.h"
#include "ui_inserttitle.h"
#include <QMessageBox>

void InsertTitle::closeEvent(QCloseEvent *event)
{

}

InsertTitle::InsertTitle(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::InsertTitle)
{
    ui->setupUi(this);
    setWindowTitle("Inserisci il titolo");
}
InsertTitle::InsertTitle(QList<QString> s,QDialog *parent) :
    QDialog(parent),
    ui(new Ui::InsertTitle)
{
    ui->setupUi(this);
    setWindowTitle("Inserisci il titolo");
    names=s;
}

InsertTitle::~InsertTitle()
{
    delete ui;
}

void InsertTitle::on_buttonBox_accepted()
{
    QString title = ui->lineEdit->text();
    bool flag=0;
    foreach (QString s, names) {
        if(s.left(s.lastIndexOf('_'))==title){
            flag=1;
            break;
    }
    }
    if ( title.size() == 0 ){
        QMessageBox::warning(this, "Titolo", "Titolo non valido");
    }
    else if(flag){
        QMessageBox::warning(this, "Titolo", "Titolo già usato");
    }
    else {
        emit setTitle(title);
        this->hide();
    }
}

void InsertTitle::on_buttonBox_rejected()
{
    emit showMw("");
    this->close();
}

