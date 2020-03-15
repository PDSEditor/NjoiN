<<<<<<< HEAD
#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    this->socketMan = new SocketManager();
    this->dbMan = new DatabaseManager();

    QObject::connect(this->socketMan, &SocketManager::newMessage, this, &Server::processMessage);


    //QObject::connect(socketMan, &SocketManager::newMessage, dbMan, &DatabaseManager::updateDB);


    //QObject::connect(this, &Server::sendMessage, socketMan, &SocketManager::messageToUser);

    //QObject::connect(dbMan, &DatabaseManager::sendFile, socketMan, &SocketManager::fileToUser );


}

void Server::dispatchMessage(Message* mes) {
    QMap<int, QWebSocket *>::iterator it = this->socketMan->getClients().begin();
    QMap<int, QWebSocket *>::iterator itEnd = this->socketMan->getClients().end();
    //std::map<int, Account>::iterator it = this->onlineAccounts.begin();
    //std::map<int, Account>::iterator itEnd = this->onlineAccounts.end();
    int sender = mes->getSymbol()->getSiteId();

    for(; it!= itEnd; it++) {
        if(it.key()!= sender)
            this->socketMan->messageToUser(mes, it.key());
    }
}

void Server::processMessage( Message* mes) {

    /* tabella di conversione:
     * Significato -> Lettera nel Messaggio -> int corrispondente
     * INSERT ->    I oppure i ->   73 oppure 105
     * DELETE ->    D oppure d ->   68 oppure 100
     * FILE REQUEST -> F oppure f
     *
    */

    QString action = mes->getAction();
    char first =  action.at(0).unicode();

    QString nomeFile;
    if (first == 'F' || first == 'f'){
        QString delimiter = "-";
        int index = action.indexOf(delimiter);
        nomeFile = action;
        nomeFile.right(index);
    }


    switch (first){
    case ('I' | 'i') :
        dbMan->insertInDB(mes);
        this->dispatchMessage(mes);
        break;
    case ('D' | 'd' ):
        dbMan->deleteFromDB(mes);
        this->dispatchMessage(mes);
        break;
    case ('F' | 'f'):
        dbMan->retrieveFile(nomeFile);
        break;
    default:
        socketMan->sendError("01 - Azione richiesta non riconosciuta");
    }

}



=======
#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    this->socketMan = new SocketManager();
    this->dbMan = new DatabaseManager();

    QObject::connect(this->socketMan, &SocketManager::newMessage, this, &Server::processMessage);


    //QObject::connect(socketMan, &SocketManager::newMessage, dbMan, &DatabaseManager::updateDB);


    //QObject::connect(this, &Server::sendMessage, socketMan, &SocketManager::messageToUser);

    //QObject::connect(dbMan, &DatabaseManager::sendFile, socketMan, &SocketManager::fileToUser );


}

void Server::dispatchMessage(Message* mes) {
    QMap<int, QWebSocket *>::iterator it = this->socketMan->getClients().begin();
    QMap<int, QWebSocket *>::iterator itEnd = this->socketMan->getClients().begin();
    //std::map<int, Account>::iterator it = this->onlineAccounts.begin();
    //std::map<int, Account>::iterator itEnd = this->onlineAccounts.end();
    int sender = mes->getSimbolo()->getSiteId();

    for(; it!= itEnd; it++) {
        if(it.key()!= sender)
            this->socketMan->messageToUser(mes, it.key());
    }
}

void Server::processMessage( Message* mes) {

    /* tabella di conversione:
     * Significato -> Lettera nel Messaggio -> int corrispondente
     * INSERT ->    I oppure i ->   73 oppure 105
     * DELETE ->    D oppure d ->   68 oppure 100
     *
    */

    char first = mes->getAction()[0];


    switch (first){
    case ('I' | 'i') :
        dbMan->insertInDB(mes);
        break;
    case ('D' | 'd' ):
        dbMan->deleteFromDB(mes);
        break;
    default:
        socketMan->sendError("01 - Azione richiesta non riconosciuta");
    }

}



>>>>>>> 3e4cc998bb356e7d0d56f393ca13f3a6e9896e64
