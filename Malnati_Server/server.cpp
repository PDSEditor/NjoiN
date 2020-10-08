#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    std::unique_ptr<SocketManager> socketMan(new SocketManager);
    this->socketMan = std::move(socketMan);

    std::unique_ptr<DatabaseManager> dbMan(new DatabaseManager);
    this->dbMan = std::move(dbMan);

    std::unique_ptr<DocumentManager> fileMan(new DocumentManager);
    this->docMan = std::move(fileMan);

    std::unique_ptr<AccountManager> acMan(new AccountManager);
    this->acMan = std::move(acMan);

    /***************************
     ****** TEST DB ***********
     *************************/

//    QString name = "test";
//    QString pass = "test";

//    QFile img("/home/pepos/projects/progett_malnati/Malnati_Server/draft.jpeg");
//    QByteArray image = img.readAll();

//    Account account(name, 0, image);
//    account.setDocumentUris({"hello", "ciao"});
//    if(this->dbMan.get()->registerAccount(account, pass, image))
//        qDebug() << "inseted?" ;



//    Account account2 = this->dbMan.get()->getAccount(QString("angelo"));
//    qDebug() << account2.toString();

//    if(this->dbMan.get()->checkAccountPsw(name,pass))
//        qDebug() << "passok" ;
//    if(this->dbMan.get()->changePassword(name, pass, "ciaone"))
//        qDebug() << "passChanged";
//    if(this->dbMan.get()->deleteAccount(name))
//        qDebug() << "deleted" ;

    /*****************************/

    QObject::connect(this->socketMan.get(), &SocketManager::newMessage, this, &Server::processMessage);
    //un nuovo utente si è collegato al server bisogna aggiungerlo a quelli online e reperire le sue informazioni
    QObject::connect(this->socketMan.get(), &SocketManager::newAccountOnline, this->acMan.get(), &AccountManager::updateOnlineAccounts );


    //QObject::connect(socketMan, &SocketManager::newMessage, dbMan, &DatabaseManager::updateDB);


    //QObject::connect(this, &Server::sendMessage, socketMan, &SocketManager::messageToUser);

    //QObject::connect(dbMan, &DatabaseManager::sendFile, socketMan, &SocketManager::fileToUser );*/


}

void Server::dispatchMessage(Message &mes) {
    QMap<int, QWebSocket*> clients = this->socketMan->getClients();
    QMap<int, QWebSocket *>::iterator it;

    int sender = mes.getSender();

    for(it=clients.begin(); it!= clients.end(); it++) {
        if(it.key() != sender)
            //qDebug()<< "Mando la remote insert al client n" << it.key();
            this->socketMan->messageToUser(mes, it.key());
    }
}

void Server::processMessage(Message &mesIn) {

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

    QChar action = mesIn.getAction();
    char first =  action.toLatin1();
    QString nomeFile;
//    int siteId;
    if (first == 'R'){
        /*QString delimiter = "-";
        int index = action.indexOf(delimiter);
        nomeFile = action;
        nomeFile.right(index);*/
    }

    QList<Symbol> document;
    QString uri;
    QString documentId;
    SharedDocument doc;
    Account acc;
    Message mesOut;
    QVector<QString> params;
    QString username;
    QList<QString> userAllowed;

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
        this->dispatchMessage(mesIn);
//        remoteInsert(mes.getSymbol());
        break;
    case 'D':
//        this->dbMan.get()->deleteSymbol(mes);
        this->dispatchMessage(mesIn);
//        remoteDelete(mes.getSymbol());
        break;
    case 'R' :

        //aggiungere il siteId tra i parametri del messaggio o assicurarsi che venga preso in altro modo

        nomeFile = mesIn.getParams()[0];
        username = mesIn.getParams()[1];

        docMan->checkPermission(username, nomeFile);
        this->dbMan.get()->retrieveSymbolsOfDocument(nomeFile);
        //Restituisci il file

        break;

    case 'C' :
    {

        if(mesIn.getParams().size()!=2) {
            qDebug()<< "Numero parametri errato nella creazione del file.";
            break;
        }

        nomeFile = mesIn.getParams()[0];
        //controllo db se esiste un file con lo stesso nome

         username = mesIn.getParams()[1];
        //creo file e lo salvo nel db con creatore = username

        userAllowed = {username};
        doc = SharedDocument(nomeFile, username, true, userAllowed);

         if(!this->dbMan->insertDocument(doc)) {
            qDebug()<< "Errore nella creazione del file.";
            break;
        }

        this->docMan->openDocument(doc);
        auto accPerFile = this->acMan->getAccountsPerFile();
        accPerFile.insert(doc.getUri(), userAllowed);
        this->acMan->setAccountsPerFile(accPerFile);

        //uso lo stesso metodo per aggiungere il creatore alla lista degli utenti associati,
        //tanto non c'è differenza lato server tra creatore e contributori
        break;
    }

    case 'X' :
        //gestire chiusura del file
        //check se il file è ancora aperto da qualcuno, se era l'unico ad averlo aperto, si procede al salvataggio su disco
        username = mesIn.getParams()[0];
        documentId = mesIn.getParams()[1];
        if(!this->acMan->closeDocumentByUser(username, documentId)) {   // se torna false, vuol dire che era l'ultimo utente con il documento aperto
            this->docMan->saveToServer(documentId);
        }

        break;

    case 'U' :
        // l'utente ha inserito un URI nell'apposito form, bisogna aggiungere il documento alla lista dei suoi documenti
        //( se esiste), aggiungere l'user negli user allowed di quel documento e caricare il documento tra quelli disponibili
        // nella pagina di scelta

        uri = mesIn.getParams()[0];
        documentId = QCryptographicHash::hash(uri.toUtf8(), QCryptographicHash::Md5);


        try {
            doc = this->dbMan->getDocument(documentId);
            int siteId = mesIn.getSender();
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

        mesOut.setAction('L');
        mesOut.setSender(mesIn.getSender());
        if(dbMan->checkAccountPsw(mesIn.getParams()[0], mesIn.getParams()[1])){
//          acc = dbMan->getAccount(mesIn.getParams()[0]);
            Account acc(dbMan->getAccount(mesIn.getParams()[0]));
            mesOut.setSender(acc.getSiteId());

            params = {acc.getUsername(), QString::number(acc.getSiteId())/*, acc.getImage()*/};
            params.append(acc.getDocumentUris().toVector());
            mesOut.setParams(params);
            mesOut.setError(false);

        }
        else {
            mesOut.setError(true);
            qDebug() << "autenticazione fallita";
        }

        socketMan->messageToUser(mesOut, mesOut.getSender());

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
    std::vector<int> index=symbol.getPosition().toStdVector();
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
    std::vector<int> index=s.getPosition().toStdVector();
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
