#include "inserttitle.h"
#include "ui_inserttitle.h"

InsertTitle::InsertTitle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InsertTitle)
{
    ui->setupUi(this);
}

InsertTitle::~InsertTitle()
{
    delete ui;
}
