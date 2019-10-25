#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <map>
#include <queue>
#include "symbol.h"
#include "sharedDocument.h"
#include "account.h"
#include "message.h"

class SharedDocument;
class Account;
class Message;

class Server: public QObject
{
Q_OBJECT

private:
    std::map<std::string, SharedDocument> documents;
    std::map<std::string, Account> onlineAccounts;
    //std::queue<Message> codaMessaggi;             Questo sarebbe il modo classico di gestirlo in c++, provare a usare invece signal e slots per gestire gli eventi


public:
    explicit Server(QObject *parent = nullptr);

signals:

public slots:
    void dispatchMessage(Message mes);
};

#endif // SERVER_H
