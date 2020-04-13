#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "mainwindow.h"
#include "socketmanager.h"
#include "Crdt.h"

class Client
{
    Q_OBJECT
public:
    Client();

private:
    MainWindow *mw;
    socketManager *sockm;
    Crdt *crdt;

};

#endif // CLIENT_H
