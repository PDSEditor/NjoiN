#ifndef ACCOUNTINTERFACE_H
#define ACCOUNTINTERFACE_H

#include <QDialog>
#include <changepwd.h>
#include <QBuffer>

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
private slots:
    void on_changePassword_clicked();
    void on_changeImage_clicked();
private:
    Ui::AccountInterface *ui;
    ChangePwd *changePwdDialog = nullptr;

signals:
    void changeImage(QByteArray&);
    void changePassword(QString, QString);
};

#endif // ACCOUNTINTERFACE_H
