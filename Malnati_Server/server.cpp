#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    std::unique_ptr<SocketManager> socketMan(new SocketManager);
    this->socketMan = std::move(socketMan);
//    this->socketMan = new SocketManager();
    std::unique_ptr<DatabaseManager> dbMan(new DatabaseManager);
    this->dbMan = std::move(dbMan);
//    this->dbMan = new DatabaseManager();
    std::unique_ptr<FileManager> fileMan(new FileManager);
    this->fileMan = std::move(fileMan);

    /***************************
     ****** TEST DB ***********
     *************************/

//    QString name = "angelo";
//    QString pass = "ciao";
//    if(this->dbMan.get()->registerUser(name, pass))
//        qDebug() << "inserted?" ;
//    if(this->dbMan.get()->checkUserPsw(name,pass))
//        qDebug() << "passok" ;
//    if(this->dbMan.get()->deleteUser(name))
//        qDebug() << "deleted" ;

    /*****************************/

    QObject::connect(this->socketMan.get(),
                     &SocketManager::newMessage,
                     this,
                     &Server::processMessage
                     );
/*//    QObject::connect(this->socketMan, &SocketManager::newMessage, this, &Server::processMessage);



    //QObject::connect(socketMan, &SocketManager::newMessage, dbMan, &DatabaseManager::updateDB);


    //QObject::connect(this, &Server::sendMessage, socketMan, &SocketManager::messageToUser);

    //QObject::connect(dbMan, &DatabaseManager::sendFile, socketMan, &SocketManager::fileToUser );*/


}

void Server::dispatchMessage(Message mes) {
    QMap<int, QWebSocket *>::iterator it = this->socketMan.get()->getClients().begin();
    QMap<int, QWebSocket *>::iterator itEnd = this->socketMan.get()->getClients().end();
    //std::map<int, Account>::iterator it = this->onlineAccounts.begin();
    //std::map<int, Account>::iterator itEnd = this->onlineAccounts.end();
    int sender = mes.getSymbol().getSiteId();

    for(; it!= itEnd; it++) {
        if(it.key()!= sender)
            this->socketMan.get()->messageToUser(mes, it.key());
    }
}

void Server::processMessage( Message mes) {

    /* tabella di conversione:
     * Significato -> Lettera nel Messaggio -> int corrispondente
     * INSERT ->    I oppure i ->   73 oppure 105
     * DELETE ->    D oppure d ->   68 oppure 100
     * FILE REQUEST -> F oppure f
     *
    */

    QChar action = mes.getAction();
    char first =  action.toLatin1();

    QString nomeFile;
    if (first == 'F' || first == 'f'){
        /*QString delimiter = "-";
        int index = action.indexOf(delimiter);
        nomeFile = action;
        nomeFile.right(index);*/
    }


    switch (first){
    case ('I' | 'i') :
        //dbMan->insertInDB(mes);
        this->dispatchMessage(mes);
        remoteInsert(mes.getSymbol());
        break;
    case ('D' | 'd' ):
        //dbMan->deleteFromDB(mes);
        this->dispatchMessage(mes);
        remoteDelete(mes.getSymbol());
        break;
    case ('F' | 'f'):
        //dbMan->retrieveFile(nomeFile);
        break;
    default:
        socketMan.get()->sendError("01 - Azione richiesta non riconosciuta");
    }

}

int compare(Symbol s1, Symbol s2){
    int len1=s1.getPosizione().size();
    int len2=s2.getPosizione().size();
    int res=0;
    for(int i=0;i<std::min(len1,len2);i++){

        if(s1.getPosizione()[i]>s2.getPosizione()[i]){
            res=1;
            break;
        }
        if(s1.getPosizione()[i]<s2.getPosizione()[i]){
            res=-1;
            break;
        }
    }
    if(res==0){
        if(len1>len2){
            res=1;
        }else
        if(len1<len2){
            res=-1;
        }

    }
    return res;
}

int Server::remoteInsert(Symbol symbol){
    int min=0;
    int max = this->symbols.size()-1;
    int middle=(max+min)/2 , pos;
    std::vector<int> index=symbol.getPosizione();
    std::vector<int> tmp;
    std::vector<Symbol>::iterator it;
    //controllo se è ultimo
    if(symbols[max].getPosizione().back()<index.front()){
        symbols.push_back(symbol);
        return max;
    }
    //controllo se è primo
    if(symbols[0].getPosizione().front()>index.back()){
        it=symbols.begin();
        symbols.insert(it,symbol);
        return min;
    }
    //è in mezzo
    while(max-min>1){
       pos=compare(symbol,symbols[middle]);
       if(pos>0){
           min=middle;
       }
       else if(pos<0){
           max=middle;
       }
       middle=(max+min)/2;
    }
    it=symbols.begin();
    pos=compare(symbol,symbols[min]);
    if(pos>0){
        //inserisco dopo il min
        symbols.insert(it+min+1,symbol);
        return min+1;
    }
    if(pos<0){
        //inserisco prima del min
        symbols.insert(it+min-1,symbol);
        return min-1;
    }
}

int Server::remoteDelete(Symbol s){
    int min=0,max=symbols.size()-1,middle=(max+min)/2,pos;
    std::vector<int> index=s.getPosizione();
    std::vector<int> tmp;
    std::vector<Symbol>::iterator it;
    it=symbols.begin();
    //controllo se è ultimo
    if(compare(s,symbols[max])==0){
            symbols.erase(it+max);
            return max;
}
    //controllo se è primo
    if(compare(s,symbols[min])==0){
            symbols.erase(it+min);
            return min;
        }
    while(max-min>1){
       pos=compare(s,symbols[middle]);
       if(pos>0){
           min=middle;
       }
       else if(pos<0){
           max=middle;
       }
       if(pos==0){
           symbols.erase(it+middle);
           break;
       }
       middle=(max+min)/2;
    }
    return middle;

}




