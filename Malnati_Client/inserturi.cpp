#include "inserturi.h"

#include "ui_inserturi.h"

#include <QCryptographicHash>

Inserturi::Inserturi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Inserturi)
{
    ui->setupUi(this);
}

Inserturi::~Inserturi()
{
    delete ui;
}

void Inserturi::on_buttonBox_clicked(QAbstractButton *button)
{
    Message m;
    m.setAction('U');
    m.setParams({ui->lineEdit->text()});
    emit(sendUri(m));
}

void Inserturi::on_buttonBox_rejected()
{
    this->close();
}
