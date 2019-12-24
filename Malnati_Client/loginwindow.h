#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    int getIsLogin() const;
    void setIsLogin(int isLogin);

private slots:
    void on_pushButton_login_clicked();

private:
    Ui::LoginWindow *ui;
    MainWindow *mw;
     int isLogin;
};

#endif // LOGINWINDOW_H
