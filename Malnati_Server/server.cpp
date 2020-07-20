#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    this->socketMan = new SocketManager();
    this->dbMan = new DatabaseManager();
    this->acMan = new AccountManager();

    //un nuovo messaggio è arrivato, deve essere processato per capire come agire
    QObject::connect(this->socketMan, &SocketManager::newMessage, this, &Server::processMessage);

    //un nuovo utente si è collegato al server bisogna aggiungerlo a quelli online e reperire le sue informazioni
    QObject::connect(this->socketMan, &SocketManager::newAccountOnline, this->acMan, &AccountManager::updateOnlineAccounts);

    //QObject::connect(socketMan, &SocketManager::newMessage, dbMan, &DatabaseManager::updateDB);


    //QObject::connect(this, &Server::sendMessage, socketMan, &SocketManager::messageToUser);

    //QObject::connect(dbMan, &DatabaseManager::sendFile, socketMan, &SocketManager::fileToUser );


}

void Server::dispatchMessage(Message* mes) {
    QMap<int, QWebSocket*> clients = this->socketMan->getClients();
    QMap<int, QWebSocket *>::iterator it;

    //std::map<int, Account>::iterator it = this->onlineAccounts.begin();
    //std::map<int, Account>::iterator itEnd = this->onlineAccounts.end();
    int sender = mes->getSymbol().getSiteId();

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
     * RETRIEVE file-> R
     * CREATE file -> C
     * CLOSE file -> X
    */

    QChar action = mes->getAction();
    char first =  action.toLatin1();

    QString nomeFile;
    int siteId;
    if (first == 'R'){
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
    case 'R' :
        //dbMan->retrieveFile(nomeFile);

        //aggiungere il siteId tra i parametri del messaggio o assicurarsi che venga preso in altro modo
        siteId = mes->getParams()[1].toInt();
        nomeFile = mes->getParams()[0];
        acMan->checkUserPerFile(siteId, nomeFile);

        //Restituisci il file
        break;

    case 'C' :
        nomeFile = mes->getParams()[0];
        //controllo db se esiste un file con lo stesso nome

        siteId = mes->getParams()[1].toInt();
        //creo file e lo salvo nel db con creatore = siteId

        //uso lo stesso metodo per aggiungere il creatore alla lista degli utenti associati,
        //tanto non c'è differenza lato server tra creatore e contributori
        acMan->checkUserPerFile(siteId, nomeFile);
        break;

    case 'X' :
        //gestire chiusura del file
        break;

    default:
        socketMan->sendError("01 - Azione richiesta non riconosciuta");
    }

}



