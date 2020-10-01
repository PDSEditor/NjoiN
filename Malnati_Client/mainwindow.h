#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "textedit.h"
#include "accountinterface.h"
#include "socketmanager.h"
#include "QBuffer"



extern socketManager *sock;
class Client;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    AccountInterface *getProfilePtr();
signals:
    void newTextEdit(TextEdit* t);
    // prova
    void sendImage(QByteArray& image);
    void sendMessage(Message* mex);

private slots:
    void receiveimage(QPixmap& im);
    void on_pushButton_clicked();
    void addElementforUser(QString);

    void on_actionNew_triggered();

    void on_actionAccount_triggered();

    void on_actionClose_triggered();

private:
    Ui::MainWindow *ui;
    TextEdit *te;
    AccountInterface *accountGui = nullptr;
    void newFile();


};
#endif // MAINWINDOW_H
