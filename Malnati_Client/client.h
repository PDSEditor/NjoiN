#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QMap>
#include "mainwindow.h"
#include "socketmanager.h"
#include "loginwindow.h"
#include "crdt.h"

class Client: public QObject
{
    Q_OBJECT
public:
    Client();
    ~Client();
    bool Login();


private slots:
    void receive_textEdit(TextEdit *t,int s);


private:
    MainWindow *mw = nullptr;
    socketManager *sockm;
    Crdt *crdt;
    bool closed=false;
    QMap<QString,TextEdit*> textList;


};

#endif // CLIENT_H

