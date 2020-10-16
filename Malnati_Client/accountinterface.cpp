#include <QIcon>
#include <QFileDialog>
#include <QMessageBox>
#include <qdebug.h>
#include "accountinterface.h"
#include "ui_accountinterface.h"

AccountInterface::AccountInterface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountInterface)
{
    ui->setupUi(this);
    this->changePwdDialog = new ChangePwd(this);
    setWindowTitle("Profile");

    connect(changePwdDialog, &ChangePwd::pwdUpdate, this, &AccountInterface::changePassword);
}

AccountInterface::~AccountInterface()
{
    delete ui;
    delete changePwdDialog;
}
void AccountInterface::setUsername(const QString& username)
{
    ui->username->setText(username);
}

void AccountInterface::setImagePic(const QPixmap &imagePic)
{
    ui->image_pic->setPixmap(imagePic.scaled(200, 200));
}

void AccountInterface::on_changePassword_clicked()
{
    this->changePwdDialog->show();
}

void AccountInterface::on_changeImage_clicked()
{
    QFileDialog file_selection(ui->image_pic);
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
    emit changeImage(pix);
}
