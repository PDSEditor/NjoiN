#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <map>
#include <queue>
#include "symbol.h"
#include "sharedDocument.h"
#include "socketManager.h"
#include "databaseManager.h"
#include "accountManager.h"
#include "documentManager.h"
#include "account.h"
#include "message.h"


class SharedDocument;
class Message;

class Server: public QObject
{
Q_OBJECT

private:
    std::unique_ptr<SocketManager> socketMan;
    std::unique_ptr<DatabaseManager> dbMan;
    std::unique_ptr<DocumentManager> docMan;
    std::unique_ptr<AccountManager> acMan;

    int remoteInsert(Symbol symbol);
    int remoteDelete(Symbol symbol);
    void updateUsersOnDocument(Message mes);

    std::vector<Symbol> symbols;
    void dispatchMessage(Message &mes);          //capisci a quali client inviare il messaggi


signals:

    void openFileFromDB (QString nome);                      // chiedi al DB manager di recuperare il file dal DB
    void closed();

public slots:
    void processMessage(Message &mes);

public:
    explicit Server(QObject *parent = nullptr);

};

#endif // SERVER_H
