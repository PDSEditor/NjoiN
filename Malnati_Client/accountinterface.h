#ifndef ACCOUNTINTERFACE_H
#define ACCOUNTINTERFACE_H

#include <QDialog>
#include <changepwd.h>
#include <QBuffer>
#include "message.h"

namespace Ui {
class AccountInterface;
}

class AccountInterface : public QDialog
{
    Q_OBJECT

public:
    explicit AccountInterface(QWidget *parent = nullptr);
    ~AccountInterface();
    void setUsername(const QString& username);
    void setImagePic(const QPixmap& imagePic);

public slots:
    void receiveNewImage(Message &);
    void receiveNewPsw(Message &);

private slots:
    void on_changePassword_clicked();
    void on_changeImage_clicked();

private:
    Ui::AccountInterface *ui;
    ChangePwd *changePwdDialog = nullptr;
    QPixmap probImage;

signals:
    void changeImage(QByteArray&);
    void changePassword(QString&, QString&);
};

#endif // ACCOUNTINTERFACE_H
