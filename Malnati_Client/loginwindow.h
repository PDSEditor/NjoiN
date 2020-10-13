#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QObject>
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
    bool getIsLogin() const;

signals:
    void sendMessage(Message* mex);
public slots:
     void receivedLogin(bool resp);
     void receivedSiteId(int siteid);
     void loggedin(bool);
private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_signup_clicked();

private:
    Ui::LoginWindow *ui;
    MainWindow *mw;
    bool isLogin;
    int siteId;
    void setIsLogin(bool isLogin);
};

#endif // LOGINWINDOW_H
