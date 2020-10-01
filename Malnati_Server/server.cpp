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

//    this->acMan = new AccountManager();
    std::unique_ptr<AccountManager> acMan(new AccountManager);
    this->acMan = std::move(acMan);

    /***************************
     ****** TEST DB ***********
     *************************/

//    QString name = "angelo";
//    QString pass = "ciao";

//    QFile img("/home/pepos/projects/progett_malnati/Malnati_Server/draft.jpeg");
//    QByteArray image = img.readAll();

//    Account account(name, 0, image);
////    account.setUsername(name);
////    account.setSiteId(0);
//    if(this->dbMan.get()->registerAccount(account, pass, image))
//        qDebug() << "inserted?" ;

//    Account account2 = this->dbMan.get()->getAccount(QString("angelo"));
//    qDebug() << account2.toString();

//    if(this->dbMan.get()->checkAccountPsw(name,pass))
//        qDebug() << "passok" ;
//    if(this->dbMan.get()->changePassword(name, pass, "ciaone"))
//        qDebug() << "passChanged";
//    if(this->dbMan.get()->deleteAccount(name))
//        qDebug() << "deleted" ;

    /*****************************/

    QObject::connect(this->socketMan.get(), &SocketManager::newMessage, this, &Server::processMessage );
    //un nuovo utente si è collegato al server bisogna aggiungerlo a quelli online e reperire le sue informazioni
    QObject::connect(this->socketMan.get(), &SocketManager::newAccountOnline, this->acMan.get(), &AccountManager::updateOnlineAccounts );


    //QObject::connect(socketMan, &SocketManager::newMessage, dbMan, &DatabaseManager::updateDB);


    //QObject::connect(this, &Server::sendMessage, socketMan, &SocketManager::messageToUser);

    //QObject::connect(dbMan, &DatabaseManager::sendFile, socketMan, &SocketManager::fileToUser );*/


}

void Server::dispatchMessage(Message &mes) {
    QMap<int, QWebSocket*> clients = this->socketMan->getClients();
    QMap<int, QWebSocket *>::iterator it;

    int sender = mes.getSymbol().getSiteId();

    for(it=clients.begin(); it!= clients.end(); it++) {
        if(it.key() != sender)
            //qDebug()<< "Mando la remote insert al client n" << it.key();
            this->socketMan->messageToUser(mes, it.key());
    }
}

void Server::processMessage( Message mes) {

    /* tabella di conversione:
     * Significato -> Lettera nel Messaggio -> int corrispondente
     * INSERT ->    I oppure i ->   73 oppure 105
     * DELETE ->    D oppure d ->   68 oppure 100
     * RETRIEVE file-> R
     * CREATE file -> C
     * CLOSE file -> X
     * Collaborate by URI -> U
     * REGISTER user (Sign up)  -> S
     * LOG-IN -> L
    */

    QChar action = mes.getAction();
    char first =  action.toLatin1();

    QString nomeFile;
    int siteId;
    if (first == 'R'){
        /*QString delimiter = "-";
        int index = action.indexOf(delimiter);
        nomeFile = action;
        nomeFile.right(index);*/
    }

    QList<Symbol> document;
//    SharedDocument sharedDocument = SharedDocument("documento1", mes.getSymbol().getSiteId());
//    QVector<QString> prova {sharedDocument.getName() + '_' + QString::number((sharedDocument.getCreator()))};
    QString uri;
    QString documentId;
    SharedDocument doc;
    Account acc;
    Message m;
    QVector<QString> params;

    switch (first){
    case 'I':
//        mes.setParams(prova);
//        dbMan->insertDocument(sharedDocument); //attenzione se già presente eccezionare
//        dbMan->insertSymbol(mes);
//        dbMan->deleteSymbol(mes);
//        sharedDocument = dbMan->getDocument(QString::fromStdString("documento1_0"));
//        document = dbMan->retrieveSymbolsOfDocument(sharedDocument.getUri()); //di test
//        for(auto i : document){
//            qDebug() << i.getValue();
//        }

//        this->dbMan.get()->insertSymbol(mes);
        this->dispatchMessage(mes);
//        remoteInsert(mes.getSymbol());
        break;
    case 'D':
//        this->dbMan.get()->deleteSymbol(mes);
        this->dispatchMessage(mes);
//        remoteDelete(mes.getSymbol());
        break;
    case 'R' :

        //aggiungere il siteId tra i parametri del messaggio o assicurarsi che venga preso in altro modo
        siteId = mes.getParams()[1].toInt();
        nomeFile = mes.getParams()[0];
        acMan->checkPermission(siteId, nomeFile);
//        this->dbMan.get()->retrieveSymbolsOfDocument(nomeFile);
        //Restituisci il file
        break;

    case 'C' :
        nomeFile = mes.getParams()[0];
        //controllo db se esiste un file con lo stesso nome

        siteId = mes.getParams()[1].toInt();
        //creo file e lo salvo nel db con creatore = siteId

        //uso lo stesso metodo per aggiungere il creatore alla lista degli utenti associati,
        //tanto non c'è differenza lato server tra creatore e contributori
        acMan->checkPermission(siteId, nomeFile);
        break;

    case 'X' :
        //gestire chiusura del file
        //check se il file è ancora aperto da qualcuno, se era l'unico ad averlo aperto, si procede al salvataggio su disco

        break;

    case 'U' :
        // l'utente ha inserito un URI nell'apposito form, bisogna aggiungere il documento alla lista dei suoi documenti
        //( se esiste), aggiungere l'user negli user allowed di quel documento e caricare il documento tra quelli disponibili
        // nella pagina di scelta
        uri = mes.getParams()[0];
        documentId = QCryptographicHash::hash(uri.toUtf8(), QCryptographicHash::Md5);

        try {
            doc = this->dbMan->getDocument(documentId);
            int siteId = mes.getSender();
            auto account = this->acMan->getOnlineAccounts().find(siteId).value();
            account.get()->getDocumentUris().push_back(uri);

            this->dbMan->addAccountToDocument(documentId, account.get()->getUsername());


        }
        catch(...) {
            qDebug() << "Documento non esistente";
        }

        break;

    case 'S' :
        //Signup

        break;

    case 'L' :
        //Login
        m.setAction('L');

        if(dbMan->checkAccountPsw(mes.getParams()[0], mes.getParams()[1])){
            acc = dbMan->getAccount(mes.getParams()[0]);
            params = {acc.getUsername(), QString::number(acc.getSiteId())/*, acc.getImage()*/};
            params.append(acc.getDocumentUris().toVector());
            m.setParams(params);
            m.setError(false);

        }
        else {
            m.setError(true)
        }

        socketMan->messageToUser(m, mes.getSender());

        break;

    default:
        this->socketMan.get()->sendError("01 - Azione richiesta non riconosciuta");
    }

}

int compare(Symbol s1, Symbol s2){
    int len1=s1.getPosition().size();
    int len2=s2.getPosition().size();
    int res=0;
    for(int i=0;i<std::min(len1,len2);i++){

        if(s1.getPosition()[i]>s2.getPosition()[i]){
            res=1;
            break;
        }
        if(s1.getPosition()[i]<s2.getPosition()[i]){
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
    std::vector<int> index=symbol.getPosition();
    std::vector<int> tmp;
    std::vector<Symbol>::iterator it;
    //controllo se è ultimo
    if(symbols[max].getPosition().back()<index.front()){
        symbols.push_back(symbol);
        return max;
    }
    //controllo se è primo
    if(symbols[0].getPosition().front()>index.back()){
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
    std::vector<int> index=s.getPosition();
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

bool Symbol::operator<(const Symbol &other) const{
    return this->getPosition() < other.getPosition();
}
