#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QMap>
#include "mainwindow.h"
#include "socketmanager.h"
#include "loginwindow.h"
#include "Crdt.h"

class Client: public QObject
{
    Q_OBJECT
public:
    Client(MainWindow* mw);
    //~Client();


private slots:
    void receive_textEdit(TextEdit *t);

private:
    MainWindow *ptrmw;
    socketManager *sockm;
    Crdt *crdt;
    QMap<QString,TextEdit*> textList;

};

#endif // CLIENT_H
