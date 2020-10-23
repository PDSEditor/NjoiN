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


    // Salvo tutti i siteId già assegnati all'interno del socket managercosì da non assegnare a due client lo stessoS

    try {
        QList<Account> allAccounts = this->dbMan->getAllAccounts();

        QMap<int, QString> siteIdUser;

        for(auto account : allAccounts) {
            siteIdUser[account.getSiteId()] = account.getUsername();
        }

        this->socketMan->setSiteIdUser(siteIdUser);

    }
    catch(std::exception& e){
    }

    /***************************
     ****** TEST DB ***********
     *************************/

//    QString name = "test";
//    QString pass = "test";

//    QFile img("/home/pepos/projects/progett_malnati/Malnati_Server/draft.jpeg");
//    QByteArray image = img.readAll();

//    Account account(name, 1, image);
//    if(this->dbMan.get()->registerAccount(account, pass))
//        qDebug() << "inserted" ;

//    QString name1 = "prova";
//    QString pass1 = "prova";


//    QFile img1("/home/pepos/projects/progett_malnati/Malnati_Server/draft.jpeg");
//    QByteArray image1 = img.readAll();

//    Account account1(name1, 5, image1);

//    if(this->dbMan.get()->registerAccount(account1, pass1))
//        qDebug() << "inserted" ;



//    Account account2 = this->dbMan.get()->getAccount(QString("test"));
//    qDebug() << account2.toString();

//    account2 = this->dbMan.get()->getAccount(QString("angelo"));
//    qDebug() << account2.toString();

//    if(this->dbMan.get()->checkAccountPsw(name,pass))
//        qDebug() << "passok" ;
//    if(this->dbMan.get()->changePassword(name, pass, "ciaone"))
//        qDebug() << "passChanged";
//    if(this->dbMan.get()->deleteAccount(name))
//        qDebug() << "deleted" ;
//        Message m1,m2;
//        Symbol s1,s2,s3,s4;
//        s1.setValue('a');
//        s1.setPosition({50});
//        s1.setSiteId(0);
//        s1.setCounter(1);
//        s2.setValue('b');
//        s2.setPosition({75});
//        s2.setSiteId(0);
//        s2.setCounter(1);
//        m1.setParams({"newfile_test"});
//        m1.setSymbol(s1);
//        m2.setParams({"newfile_test"});
//        m2.setSymbol(s2);
//            if(this->dbMan.get()->insertSymbol(m1))
//                qDebug() << "symbol inserted" ;
//            if(this->dbMan.get()->insertSymbol(m2))
//                qDebug() << "symbol inserted" ;

    /*****************************/

    QObject::connect(this->socketMan.get(), &SocketManager::newMessage, this, &Server::processMessage);
    //un nuovo utente si è collegato al server bisogna aggiungerlo a quelli online e reperire le sue informazioni
    //QObject::connect(this, &Server::newAccountOnline, this->acMan.get(), &AccountManager::updateOnlineAccounts );
    QObject::connect(this->socketMan.get(), &SocketManager::accountDisconnected, this->acMan.get(), &AccountManager::removeOnlineAccounts );


}

void Server::dispatchMessage(Message &mes) {
    QMap<int, QWebSocket*> clients = this->socketMan->getClients();
    QMap<int, QWebSocket *>::iterator it;

    int sender = mes.getSender();
    //QString documentId = mes.getParams()[0];

    QString user = this->acMan->getOnlineAccounts()[sender].get()->getUsername();

    QString documentId = this->acMan->getUsernameDocumentMap()[user];

    for(it=clients.begin(); it!= clients.end(); it++) {
        if(it.key() != sender || mes.getAction()=='A') {

            QString username = this->acMan->getOnlineAccounts()[it.key()].get()->getUsername();    // prende l'username legato al siteId del messaggio

            if(this->acMan->getAccountsPerFile()[documentId].contains(username)) {                 // controlla se l'utente trovato è in quelli che stanno attualmente
                                                                                       // lavorando al documento, in quel caso invia la insert o delete
                this->socketMan->messageToUser(mes, it.key());
            }
        }
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
     * REGISTER user  -> E
     * LOG-IN -> L
     * LOGOUT -> O
     * ACCOUNTS ON FILE -> A
    */

    QChar action = mesIn.getAction(),type;
    char first =  action.toLatin1();
    QString nomeFile;
    QList<Symbol> document;
    QString uri;
    QString documentId;
    SharedDocument doc;
    Account acc;
    Message mesOut;
    QVector<QString> params;
    QString username;
    QList<QString> userAllowed;
    int start, end;

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

        username = this->acMan->getOnlineAccounts()[mesIn.getSender()]->getUsername();
        documentId = this->acMan->getUsernameDocumentMap()[username];
        mesIn.setParams({documentId});
        this->dbMan.get()->insertSymbol(mesIn);
        this->dispatchMessage(mesIn);
//        remoteInsert(mesIn.getSymbol());
        break;
    case 'D':
        username = this->acMan->getOnlineAccounts()[mesIn.getSender()]->getUsername();
        documentId = this->acMan->getUsernameDocumentMap()[username];
        mesIn.setParams({documentId});

        this->dbMan.get()->deleteSymbol(mesIn);
        this->dispatchMessage(mesIn);
//        remoteDelete(mesIn.getSymbol());
        break;



    case 'R' :

        documentId = mesIn.getParams()[0];
        username = mesIn.getParams()[1];
        mesOut.setAction('R');
        mesOut.setSender(mesIn.getSender());

        doc = this->dbMan->getDocument(documentId);

        if(doc.getUserAllowed().contains(username)){
            auto symbols = this->dbMan.get()->retrieveSymbolsOfDocument(documentId);
            for(auto symbol : symbols) {
                mesOut.addParam(symbol.toJson().toJson(QJsonDocument::Compact));
            }
            mesOut.setError(false);

            this->acMan->updateAccountOnDocument(username, documentId);

            auto accPerFile = this->acMan->getAccountsPerFile();
            accPerFile[documentId].append(username);
            accPerFile.insert (documentId, accPerFile[documentId]);
            this->acMan->setAccountsPerFile(accPerFile);

            this->docMan->openDocument(doc);

        }
        else{
            mesOut.setError(true);              //Non autorizzato
        }



        socketMan->messageToUser(mesOut, mesOut.getSender());

        this->updateUsersOnDocument(mesIn);

        break;



    case 'C' :
    {

//        if(mesIn.getParams().size()!=2) {
//            qDebug()<< "Numero parametri errato nella creazione del file.";
//            break;
//        }

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

        this->acMan->updateAccountOnDocument(username, (nomeFile +"_"+username));

        this->docMan->openDocument(doc);

        //uso lo stesso metodo per aggiungere il creatore alla lista degli utenti associati,
        //tanto non c'è differenza lato server tra creatore e contributori
        break;
    }

    case 'X' :
        //gestire chiusura del file
        //check se il file è ancora aperto da qualcuno, se era l'unico ad averlo aperto, si procede al salvataggio su disco
        username = mesIn.getParams()[1];
        documentId = mesIn.getParams()[0];
        if(!this->acMan->closeDocumentByUser(username, documentId)) {   // se torna false, vuol dire che era l'ultimo utente con il documento aperto

            this->docMan->closeDocument(documentId);

            // per ora commentato
            //this->docMan->saveToServer(documentId);
        }


        break;

    case 'U' :
        // l'utente ha inserito un URI nell'apposito form, bisogna aggiungere il documento alla lista dei suoi documenti
        //( se esiste), aggiungere l'user negli user allowed di quel documento e caricare il documento tra quelli disponibili
        // nella pagina di scelta

        uri = mesIn.getParams()[0];
       // documentId = QCryptographicHash::hash(uri.toUtf8(), QCryptographicHash::Md5);
        documentId=uri;


        mesOut.setAction('U');
        mesOut.setSender(mesIn.getSender());

        try {
            doc = this->dbMan->getDocument(documentId);
            int siteId = mesIn.getSender();
            auto account = this->acMan->getOnlineAccounts().find(siteId).value();
            account.get()->getDocumentUris().push_back(uri);

            /** aggiorniamo entrambe le liste **/
            this->dbMan->addAccountToDocument(documentId, account.get()->getUsername());
            this->dbMan->addDocumentToAccount(documentId, account.get()->getUsername());

            auto symbols = this->dbMan.get()->retrieveSymbolsOfDocument(documentId);
            for(auto symbol : symbols) {
                mesOut.addParam(symbol.toJson().toJson(QJsonDocument::Compact));
            }
            mesOut.setError(false);

            this->acMan->updateAccountOnDocument(account.get()->getUsername(), uri);

            auto accPerFile = this->acMan->getAccountsPerFile();
            accPerFile[documentId].append((account.get()->getUsername()));
            accPerFile.insert (documentId, accPerFile[documentId]);
            this->acMan->setAccountsPerFile(accPerFile);

        }
        catch(std::exception& e){
            qDebug() << "Documento non esistente";
            mesOut.setError(true);
        }

        socketMan->messageToUser(mesOut, mesOut.getSender());

        this->updateUsersOnDocument(mesIn);
        this->docMan->openDocument(doc);

        break;

    case 'E' :
        //rEgister

        username = mesIn.getParams()[0];
        mesOut.setAction('E');
        mesOut.setSender(mesIn.getSender());

        acc = this->dbMan->getAccount(username);

        if( acc.getSiteId()< 0) {               // non esiste un account con questo username
            mesOut.setError(false);

            acc = Account(username, mesIn.getSender());

            this->dbMan->registerAccount(acc, mesIn.getParams()[1]);
            auto a=this->socketMan->getSiteIdUser();
            a[acc.getSiteId()]=username;
            this->socketMan->setSiteIdUser(a);

        }
        else {
            mesOut.setError(true);
        }

        socketMan->messageToUser(mesOut, mesOut.getSender());

        break;

    case 'L' :
        //Login

        mesOut.setAction('L');
        mesOut.setSender(mesIn.getSender());
        if(dbMan->checkAccountPsw(mesIn.getParams()[0], mesIn.getParams()[1])){

            Account acc(dbMan->getAccount(mesIn.getParams()[0]));

            if(this->acMan->updateOnlineAccounts(acc.getSiteId(), acc)) {               //utente collegato correttamente
                params = {acc.getUsername(), QString::number(acc.getSiteId())/*, acc.getImage()*/};
                params.append(acc.getDocumentUris().toVector());
                mesOut.setParams(params);
                mesOut.setError(false);
                mesOut.setSender(acc.getSiteId());           }
            else {                                                                      //utente era già collegato da un altro client
                mesOut.setError(true);
                qDebug() << "autenticazione di un utente già online";
                mesOut.setParams({"2"});
            }

        }
        else {
            mesOut.setError(true);
            qDebug() << "autenticazione fallita";
            mesOut.setParams({"1"});
        }

        socketMan->messageToUser(mesOut, mesIn.getSender());                       // qui mando il mesOut con dentro il sender temporaneo
                                                                                    // e dentro il siteId ci metto il sender "ufficiale"

        break;

    case 'O' :
        //Logout
        this->acMan->removeOnlineAccounts(mesIn.getSender());                       // il metodo si occupa di cancellare l'account da tutte le liste di account online

        break;

    case 'A' :
       //Recupera la lista degli utenti attualmente in lavorazione sul file

        this->updateUsersOnDocument(mesIn);

        break;

    case 'B' :

        start = mesIn.getParams()[0].toInt();
        end = mesIn.getParams()[1].toInt();
        type = mesIn.getParams()[2].at(0);

        username = this->acMan->getOnlineAccounts()[mesIn.getSender()]->getUsername();

        documentId = this->acMan->getUsernameDocumentMap()[username];

        document = this->dbMan->retrieveSymbolsOfDocument(documentId);

        for (int i=start; i<= end; i++) {
           document[i].setAlign(type);
        }

        this->dbMan->setSymbolsOfDocument(documentId, document);

        this->dispatchMessage(mesIn);

        break;

    default:
        this->socketMan.get()->sendError("01 - Azione richiesta non riconosciuta");
    }

}

void Server::updateUsersOnDocument(Message mes)
{
    QString documentId=mes.getParams()[0];
    mes.setAction('A');

    QVector<QString> onlineUsers = this->acMan->getAccountsPerFile()[documentId].toVector();
    QVector<QString> onlineUsers_siteId;

    auto siteIdUser = this->socketMan->getSiteIdUser();

    if(!onlineUsers.contains(siteIdUser[mes.getSender()]))
        onlineUsers.append(siteIdUser[mes.getSender()]);

    for(auto user: onlineUsers){

        for (auto it = siteIdUser.begin(); it !=siteIdUser.end(); it++) {
            if(it.value() == user)
                onlineUsers_siteId.append(user+"_"+QString::number(it.key()));
        }
    }

    onlineUsers_siteId.append("___");           // separator beetween online and offline users

    QVector<QString> offlineUsers_siteId;

    for(auto user : this->dbMan->getDocument(documentId).getUserAllowed()) {
        if(!onlineUsers.contains(user))          //se lo user non è tra quelli online (params) allora lo aggiungo tra quelli offline

            for (auto it = siteIdUser.begin(); it != siteIdUser.end(); it++) {
                if(it.value() == user)
                    offlineUsers_siteId.append(user+"_"+QString::number(it.key()));
            }

    }

    onlineUsers_siteId.append(offlineUsers_siteId);

    mes.setParams(onlineUsers_siteId);

    //socketMan->messageToUser(mes, mes.getSender());
    this->dispatchMessage(mes);

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
    std::vector<int> index(symbol.getPosition().begin(), symbol.getPosition().end());//=symbol.getPosition().toStdVector();
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
    std::vector<int> index(s.getPosition().begin(), s.getPosition().end());//=s.getPosition().toStdVector();
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
