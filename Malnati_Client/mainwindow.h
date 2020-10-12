﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "textedit.h"
#include "accountinterface.h"
#include "socketmanager.h"
#include "QBuffer"
#include <QListWidgetItem>
#include <inserturi.h>



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
    void setImage(QPixmap im);
    QPixmap getImage();
    void setUsername(QString username);
    QString getUsername();
    QList<QString> getList();
    void setList(QList<QString> l);
    int getSiteId() const;
    void setSiteId(int value);
    void setURI(QString);
    QString getURI();

public slots:
    void receivedInfoAccount(Message& m);
    void receivedFile(QList<Symbol>);
    void sendUri(Message);
    void receiveURIerror();

signals:
    void newTextEdit(TextEdit* t);
    // prova
    void sendImage(QByteArray& image);
    void sendMessage(Message* mex);
    void sendTextMessage(Message* mex);


private slots:
    void receiveimage(QPixmap& im);
    void on_pushButton_clicked();
    void addElementforUser(QString);
    void on_actionNew_triggered();
    void on_actionAccount_triggered();
    void on_actionClose_triggered();
    void open_file_on_server(QListWidgetItem*);

    void on_pushButton_2_clicked();

    void receiveTitle(QString title);

    void openMw();

private:
    Ui::MainWindow *ui;
    TextEdit *te;
    AccountInterface *accountGui = nullptr;
    void newFile();
    bool flaglocal=0;
    /** Account **/
    QPixmap image;
    QString username,openURI;
    QList<QString> documents;
    int siteId;
    /*************/

};
#endif // MAINWINDOW_H
