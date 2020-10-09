#include "inserttitle.h"
#include "ui_inserttitle.h"
#include <QMessageBox>

InsertTitle::InsertTitle(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::InsertTitle)
{
    ui->setupUi(this);
    setWindowTitle("Inserisci il titolo");
}

InsertTitle::~InsertTitle()
{
    delete ui;
}

void InsertTitle::on_buttonBox_accepted()
{
    QString title = ui->lineEdit->text();
    if ( title.size() == 0 ){
        QMessageBox::warning(this, "Titolo", "Titolo non valido");
    } else {
        emit setTitle(title);
        this->hide();
    }
}

void InsertTitle::on_buttonBox_rejected()
{
    emit showMw();
    this->close();
}

