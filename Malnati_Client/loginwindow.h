#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    int getIsLogin() const;


private slots:
    void on_pushButton_login_clicked();

private:
    Ui::LoginWindow *ui;
    MainWindow *mw;
     int isLogin;
     void setIsLogin(int isLogin);
};

#endif // LOGINWINDOW_H
