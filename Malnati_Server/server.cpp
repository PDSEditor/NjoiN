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
    QMap<int, QWebSocket*> clients = this->socketMan->getClients();
    QMap<int, QWebSocket *>::iterator it;

    //std::map<int, Account>::iterator it = this->onlineAccounts.begin();
    //std::map<int, Account>::iterator itEnd = this->onlineAccounts.end();
    int sender = mes->getSymbol()->getSiteId();

    for(it=clients.begin(); it!= clients.end(); it++) {
        if(it.key() != sender)
            //qDebug()<< "Mando la remote insert al client n" << it.key();
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

    QChar action = mes->getAction();
    char first =  action.toLatin1();

    QString nomeFile;
    if (first == 'F' || first == 'f'){
        /*QString delimiter = "-";
        int index = action.indexOf(delimiter);
        nomeFile = action;
        nomeFile.right(index);*/
    }


    switch (first){
    case 'I':
        //dbMan->insertInDB(mes);
        this->dispatchMessage(mes);
        break;
    case 'D':
        //dbMan->deleteFromDB(mes);
        this->dispatchMessage(mes);
        break;
    case 'F' :
        //dbMan->retrieveFile(nomeFile);
        break;
    default:
        socketMan->sendError("01 - Azione richiesta non riconosciuta");
    }

}



