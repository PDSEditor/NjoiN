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
        qDebug() << "error, " << e.what();
        throw e;
    }

    /***************************
     ****** TEST DB ***********
     *************************/

//    QString name = "test";
//    QString pass = "test";

//    QFile img1("/home/pepos/projects/progett_malnati/Malnati_Server/draft.jpeg");
//    QByteArray image = img1.readAll();

//    Account account(name, 1, image);
//    if(this->dbMan.get()->registerAccount(account, pass))
//        qDebug() << "inserted" ;

//    QString name1 = "prova";
//    QString pass1 = "prova";


//    QFile img2("/home/pepos/projects/progett_malnati/Malnati_Server/draft.jpeg");
//    QByteArray image1 = img2.readAll();

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
            if(this->acMan->getOnlineAccounts().keys().contains(it.key())){
                QString username = this->acMan->getOnlineAccounts()[it.key()].get()->getUsername();    // prende l'username legato al siteId del messaggio

                if(this->acMan->getAccountsPerFile()[documentId].contains(username)) {                 // controlla se l'utente trovato è in quelli che stanno attualmente
                    // lavorando al documento, in quel caso invia la insert o delete
                    this->socketMan->messageToUser(mes, it.key());
                }
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
     * CAMBIO IMMAGINE ACCOUNT -> G
     * CAMBIO PASSWORD ACCOUNT -> P
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
    {
        username = this->acMan->getOnlineAccounts()[mesIn.getSender()]->getUsername();
        documentId = this->acMan->getUsernameDocumentMap()[username];
        mesIn.setParams({documentId});
        try {
            this->dbMan.get()->insertSymbol(mesIn);
        } catch (std::exception& e) {
            qDebug()<<"Fallito inserimento di simbolo";
        }

        this->dispatchMessage(mesIn);
        break;
    }
    case 'D':{
        username = this->acMan->getOnlineAccounts()[mesIn.getSender()]->getUsername();
        documentId = this->acMan->getUsernameDocumentMap()[username];
        mesIn.setParams({documentId});

        try {
            this->dbMan.get()->deleteSymbol(mesIn);
        } catch (std::exception& e) {
            qDebug()<<"Fallita cancellazione del simbolo";
        }

        this->dispatchMessage(mesIn);
        break;
    }

    case 'R' :{

        documentId = mesIn.getParams()[0];
        username = mesIn.getParams()[1];
        mesOut.setAction('R');
        mesOut.setSender(mesIn.getSender());

        try {
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
           if(!mesOut.getError()){
                this->updateUsersOnDocument(mesIn);
            }


        } catch (std::exception& e) {
            qDebug()<<"Errore nella retrieve del documento";
        }

        break;
    }

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

        try {
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

        } catch (std::exception& e) {
            qDebug()<< "Errore nella create del file";
        }


        break;
    }

    case 'X' :{
        //gestire chiusura del file
        //check se il file è ancora aperto da qualcuno, se era l'unico ad averlo aperto, si procede al salvataggio su disco
        username = mesIn.getParams()[1];
        documentId = mesIn.getParams()[0];

        try {
            if(!this->acMan->closeDocumentByUser(username, documentId)) {   // se torna false, vuol dire che era l'ultimo utente con il documento aperto
                auto symbols = this->dbMan->retrieveSymbolsOfDocument(documentId);
                if(!this->docMan->saveToServer(documentId, symbols)){
                    qDebug() << "Errore nel salvataggio in locale sul server del file";
                }
                this->docMan->closeDocument(documentId);
            }
        } catch (std::exception& e) {
            qDebug()<< "Errore nella chiusura di un documento";
        }

        break;
    }

    case 'U' :{
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


            this->docMan->openDocument(doc);

        }
        catch(std::exception& e){
            qDebug() << "Documento non esistente";
            mesOut.setError(true);
        }

        socketMan->messageToUser(mesOut, mesOut.getSender());

        if(!mesOut.getError()){
             this->updateUsersOnDocument(mesIn);
         }

        break;
    }

    case 'E' :{
        //rEgister

        username = mesIn.getParams()[0];
        mesOut.setAction('E');
        mesOut.setSender(mesIn.getSender());

        QByteArray img = mesIn.getParams()[2].toLatin1();

        try {
            Account acc(this->dbMan->getAccount(username));

            if( acc.getSiteId()< 0) {               // non esiste un account con questo username
                mesOut.setError(false);

    //            acc = Account(username, mesIn.getSender());
                acc = Account(username, mesIn.getSender(), img);

                this->dbMan->registerAccount(acc, mesIn.getParams()[1]);
                auto a=this->socketMan->getSiteIdUser();
                a[acc.getSiteId()]=username;
                this->socketMan->setSiteIdUser(a);

            }
            else {
                mesOut.setError(true);
            }

            socketMan->messageToUser(mesOut, mesOut.getSender());

        } catch (std::exception& e) {
            qDebug()<<"Errore nella registrazione";
        }

        break;
    }

    case 'L' :{
        //Login

        mesOut.setAction('L');
        mesOut.setSender(mesIn.getSender());

        auto username = mesIn.getParams()[0];
        auto password = mesIn.getParams()[1];

        try {
            if(dbMan->checkAccountPsw(username, password)){

                Account acc(dbMan->getAccount(username));

                if(this->acMan->updateOnlineAccounts(acc.getSiteId(), acc)) {               //utente collegato correttamente
                    params = {acc.getUsername(), QString::number(acc.getSiteId()), acc.getImage()};
                    params.append(acc.getDocumentUris().toVector());
                    mesOut.setParams(params);
                    mesOut.setError(false);
                    mesOut.setSender(acc.getSiteId());
                }else {                                                                      //utente era già collegato da un altro client
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
        } catch (std::exception& e) {
            qDebug()<< "Errore nella Login";
        }


        break;
    }

    case 'O' :{
        //Logout
        this->acMan->removeOnlineAccounts(mesIn.getSender());                       // il metodo si occupa di cancellare l'account da tutte le liste di account online

        break;
    }

    case 'A' :{
       //Recupera la lista degli utenti attualmente in lavorazione sul file

        this->updateUsersOnDocument(mesIn);

        break;
    }

    case 'B' :{

        start = mesIn.getParams()[0].toInt();
        end = mesIn.getParams()[1].toInt();
        type = mesIn.getParams()[2].at(0);

        try {
            username = this->acMan->getOnlineAccounts()[mesIn.getSender()]->getUsername();

            documentId = this->acMan->getUsernameDocumentMap()[username];

            document = this->dbMan->retrieveSymbolsOfDocument(documentId);

            for (int i=start; i<= end; i++) {
               document[i].setAlign(type);
            }

            this->dbMan->setSymbolsOfDocument(documentId, document);

            this->dispatchMessage(mesIn);

        } catch (std::exception) {
            qDebug() <<"Errore nel tentativo di allineare una selezione";
        }


        break;
    }

    case 'Z' :{

        this->dispatchMessage(mesIn);

        break;

    }

    case 'G':{
        //cambio immagine
        username = mesIn.getParams()[0];
        mesOut.setAction('G');
        mesOut.setSender(mesIn.getSender());

        QByteArray img = mesIn.getParams()[1].toLatin1();

        try {
            Account acc(this->dbMan->getAccount(username));

            if(acc.getSiteId()<0)
                mesOut.setError(true);

            if(this->dbMan->changeImage(username, img)){
                mesOut.setError(false);
            }else mesOut.setError(true);

            socketMan->messageToUser(mesOut, mesOut.getSender());
        } catch (std::exception& e) {
            qDebug()<<"Errore nel cambiamento dell'immagine";
        }


        break;
    }
    case 'P':{
        //cambio password
        username = mesIn.getParams()[0];
        QString oldPsw = mesIn.getParams()[1];
        QString newPsw = mesIn.getParams()[2];
        mesOut.setAction('P');
        mesOut.setSender(mesIn.getSender());

        try {
            Account acc(this->dbMan->getAccount(username));

            if(acc.getSiteId()<0)
                mesOut.setError(true);

            if(this->dbMan->changePassword(username, oldPsw, newPsw))
                mesOut.setError(false);
            else mesOut.setError(true);

            socketMan->messageToUser(mesOut, mesOut.getSender());
        } catch (std::exception& e) {
            qDebug()<<"Errore nel cambiamento della password";
        }

        break;
    }

    default:{
        this->socketMan.get()->sendError("01 - Azione richiesta non riconosciuta");
    }
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

    try {
        auto userAllowed = this->dbMan->getDocument(documentId).getUserAllowed();
        for(auto user : userAllowed ) {
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

    }catch(std::exception& e){
        qDebug() << "Documento non esistente";
    }

}

bool Symbol::operator<(const Symbol &other) const{
    return this->getPosition() < other.getPosition();
}
