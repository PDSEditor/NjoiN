#include "inserttitle.h"
#include "ui_inserttitle.h"
#include <QMessageBox>

InsertTitle::InsertTitle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InsertTitle)
{
    ui->setupUi(this);
    setWindowTitle("Insert Title");
}

InsertTitle::~InsertTitle()
{
    delete ui;
}

void InsertTitle::on_buttonBox_accepted()
{
    QString title = ui->lineEdit->text();
    if ( title.size() == 0 ){
        QMessageBox::warning(this, "Title", "Title not valid");
    } else {
        emit setTitle(title);
        this->hide();
    }
}

void InsertTitle::on_buttonBox_rejected()
{
    this->hide();
}

