#include "socketManager.h"
#include "QtWebSockets/qwebsocketserver.h"


SocketManager::SocketManager(QObject *parent) : QObject(parent),
    qWebSocketServer(new QWebSocketServer(QStringLiteral("Server Shared Editor"),
                                          QWebSocketServer::NonSecureMode, this)){

        SocketManager::siteId = 0;

        if (qWebSocketServer->listen(QHostAddress::Any, N_PORT)) {   //ascolta su tutte le interfacce, posta scelta automaticamente

            //if (m_debug)
            //    qDebug() << "Echoserver listening on port" << port;
            connect(qWebSocketServer, &QWebSocketServer::newConnection,
                    this, &SocketManager::onNewConnection);

            //connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &EchoServer::closed);


        }
}

void SocketManager::sendError(std::string)
{

}

void SocketManager::messageToUser( Message &m, int siteId) {

   //TODO: invia il singolo messaggio ai vari client

    auto it = this->clients.find(siteId);
    if (it != clients.end()) {
        if(m.getAction()=='S') {
            qDebug()<<"Invio site id al client n "<< siteId;
        }
        if(m.getAction() == 'L') {                          //Se sono nel caso del login, devo prendere il siteId a cui viene associato il socket
                                                            // che è temporaneo e associarlo al vero siteId che ho nel server, in questo modo
                                                            // da ora in poi userò solo il vero siteId

           auto const elem = std::move(it.value());
           this->clients.erase(it);
           this->clients.insert(m.getSender(), std::move(elem));
        }
        QWebSocket *user = it.value();
        if(m.getAction()=='I' || m.getAction()=='D') {
            binaryMessageToUser(m, siteId);
        }
        else {

            user->sendTextMessage(m.toJson().toJson(QJsonDocument::Compact));
        }

    }
}

void SocketManager::binaryMessageToUser(Message &m, int siteId)
{
    int tmp;
    QChar tmpc;
    QByteArray bytemex;
    QChar action = m.getAction();
    Symbol symbol = m.getSymbol();
    QVector<QString> params = m.getParams();


    if(action==("I")||action==("D")){
        if(action==("I")){
            bytemex.append('I');
        }
        else{
            bytemex.append('D');
        }

        bytemex.append('{');
        for(auto i=0; i<symbol.getPosition().size(); i++){
            tmp=(symbol.getPosition().at(i));

            for(int p=0;p<4;p++){
                bytemex.append(tmp >> (p * 8));
            }
        }

        bytemex.append('}');
        bytemex.append(symbol.getSiteId());//dimensione massima
        tmp=(symbol.getCounter());

        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }

        /*for(int p=0;p<2;p++){
            bytemex.append(tmp >> (p*8));
        }*/
        tmpc=symbol.getValue();
        bytemex.append(tmpc.cell());
        bytemex.append(tmpc.row());

        bytemex.append(symbol.getBold());
        bytemex.append(symbol.getUnderln());
        bytemex.append(symbol.getItalic());
        tmp=symbol.getSize();
        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
        tmp=m.getSender();
        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
        bytemex.append(symbol.getFamily());

    }
//    else if(action==('C')||action==('R')){
//        if(action==('C')){
//            bytemex.append('C');
//        }
//        else{
//            bytemex.append('R');
//        }
//        tmp=siteId;
//        for(int p=0;p<4;p++){
//            bytemex.append(tmp >> (p * 8));
//        }
//        bytemex.append(params.at(0));

//    }
//    else if(action=='L'){
//        bytemex.append('L');
//        tmp=siteId;
//        for(int p=0;p<4;p++){
//            bytemex.append(tmp >> (p * 8));
//        }
//        tmp=params.at(0).length();
//        for(int p=0;p<4;p++){
//            bytemex.append(tmp >> (p * 8));
//        }
//        bytemex.append(params.at(0));
//        tmp=params.at(1).length();
//        for(int p=0;p<4;p++){
//            bytemex.append(tmp >> (p * 8));
//        }
//        bytemex.append(params.at(1));
//    }
//    else if (action == 'S') {
//        bytemex.append('S');
//        tmp=siteId;
//        for(int p=0;p<4;p++){
//            bytemex.append(tmp >> (p * 8));
//        }
//        tmp=params.at(0).length();
//        for(int p=0;p<4;p++){
//            bytemex.append(tmp >> (p * 8));
//        }
//        bytemex.append(params.at(0));
//    }

    //qDebug()<<'lunghezza array di byte'<<bytemex.size();
    //webSocket.sendBinaryMessage( bytemex);
    auto it = this->clients.find(siteId);
    if (it != clients.end()) {
        QWebSocket *user = it.value();
        user->sendBinaryMessage(bytemex);
    }
}


void SocketManager::fileToUser(std::vector<Symbol> &file, int user)
{

}

void SocketManager::processTextMessage(QString message)
{

    //QWebSocket *client = qobject_cast<QWebSocket *>(sender());    probabilmente non serve, il sender è già identificato tramite SiteId
    //qDebug()<<message;

    //deserialize JSON
//    Message m = Message::fromJson(QJsonDocument::fromJson(message.toUtf8()));
    auto document = QJsonDocument::fromJson(message.toUtf8());
    Message m = Message::fromJson(document);

    emit newMessage(m);

}

void SocketManager::processBinaryMessage(const QByteArray &bytemex)
{
    qDebug()<<bytemex;
    //Qui devo differenziare i byte tra action param e symbol
    /*
     * Messaggio:
     * il caratter 1 rappresenta l'azione
     *  Se 1 == I =73 (insert) o == D =68 (delete)
     *      allora il messaggio sarà composto nella seguente maniera "[?]??"
     *      dove i punti interrogativi sono nell'ordine posizione, siteid e counter
     *  Se 1 == R =82 (retrieve) o == C =67(create)
     *      allora il secondo carattere è un numero che rappresenta la lunghezza del campo nome file.
     *      A seguire ci saranno quindi altrettanti caratteri che rappresentano il nome del file.
     *  Se 1 == L =76(login)
     *      allora il secondo carattere è un numero che rappresenta la lunghezza del campo username.
     *      A seguire ci saranno quindi altrettanti caratteri che rappresentano lo username.
     *      Alla fine di nuovo un numero che rappresenta la lunghezza della password
     *      A seguire ci saranno quindi altrettanti caratteri che rappresentano la password.
     *  Se 1 = T =84(test)
     *      stampa semplicemente output
     */

    bool it,un,bo;
    QString family;
    QByteArray c;
    int tmp,d,sender;
    QChar tmpc;
    QChar action;
    Symbol symbol;
    QVector<QString> params;
    action=bytemex.at(0);
    if(bytemex.at(0)=='I'||bytemex.at(0)=='D'){
        if(bytemex.at(0)=='I')
            action='I';
        else
            action='D';
        std::vector<int> vtmp;
        int i=2;
        while(bytemex.at(i)!='}'){
            c.clear();
            c.append(bytemex.mid(i,4));
            memcpy(&tmp,c,4);
            vtmp.push_back(tmp);
            i+=4;
        }
        i++;
        symbol.setPosition(vtmp);
        symbol.setSiteId((int)bytemex.at(i++));
        c.clear();
        c.append(bytemex.mid(i,4));
        memcpy(&tmp,c,4);
        symbol.setCounter(tmp);
        i+=4;
        c.clear();
        c.append(bytemex.mid(i,2));
        memcpy(&tmpc,c,2);
        symbol.setValue(tmpc);
        //symbol.setValue(bytemex.at(i));
        i+=2;
        bo=bytemex.at(i);
        i++;
        un=bytemex.at(i);
        i++;
        it=bytemex.at(i);
        i++;
        c.clear();
        c.append(bytemex.mid(i,4));
        memcpy(&tmp,c,4);
        d=tmp;
        i+=4;
        c.clear();
        c.append(bytemex.mid(i,4));
        memcpy(&tmp,c,4);
        sender=tmp;
        i+=4;
        family=bytemex.right(bytemex.length()-i);
    }
    else if(bytemex.at(0)=='C'||bytemex.at(0)=='R'){
        if(bytemex.at(0)=='C')
            action='C';
        else
            action='R';
        c.clear();
        c.append(bytemex.mid(1,4));
        memcpy(&tmp,c,4);
        sender=tmp;
        params.push_back(bytemex.right(bytemex.length()-5));
    }
    else if(bytemex.at(0)=='L'){
        action='L';
        c.clear();
        c.append(bytemex.mid(1,4));
        memcpy(&tmp,c,4);
        sender=tmp;
        c.clear();
        c.append(bytemex.mid(5,4));
        memcpy(&tmp,c,4);
        params.push_back(bytemex.mid(9,tmp));
        c.clear();
        c.append(bytemex.mid(tmp+9,4));
        memcpy(&tmp,c,4);
        params.push_back(bytemex.right(tmp));
    }

    Message m;
    m.setAction(action);
    m.setParams(params);

    symbol.setBold(bo);
    symbol.setSize(d);
    symbol.setItalic(it);
    symbol.setUnderln(un);
    symbol.setFamily(family);
    m.setSymbol(symbol);
    m.setSender(sender);
    /** necessario settare i decorators? **/


    m.debugPrint();
    //
//    this->binaryMessageToUser(m,0);

    emit newMessage(m);

}


QMap<int, QWebSocket *> SocketManager::getClients() const
{
    return clients;
}

void SocketManager::setClients(const QMap<int, QWebSocket *> &value)
{
    clients = value;
}

void SocketManager::onNewConnection()
{
    QWebSocket *socket = qWebSocketServer->nextPendingConnection();

    qDebug()<<"Nuova connessione avvenuta.";


    connect(socket, &QWebSocket::textMessageReceived, this, &SocketManager::processTextMessage);
    connect(socket, &QWebSocket::binaryMessageReceived, this, &SocketManager::processBinaryMessage);
    connect(socket, &QWebSocket::disconnected, this, &SocketManager::socketDisconnected);

    while(this->siteIdUser.keys().contains(this->siteId)){      //controlla se il siteId attuale corrisponde già a qualche altro utente
        this->siteId++;
    }

    clients.insert(SocketManager::siteId, socket);


    //successivamente comunicare al client il proprio siteId
    Message m;

    m.setAction('S');



    QString s = QString::number(SocketManager::siteId);
    m.setParams({s});
    m.setSender(SocketManager::siteId);
    messageToUser(m,SocketManager::siteId);

    SocketManager::siteId++;
}

void SocketManager::socketDisconnected()
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());

    int siteId = -1;

    if (client) {
        auto it = clients.begin();
        while (it!= clients.end()) {
            if(it.value()==client){
                siteId = it.key();
                it = clients.erase(it);
            }
            else
                it++;
        }
        client->deleteLater();
    }

    //emetti segnale per rimuovere da onlineAccounts
    if(siteId >=0) {
        emit(accountDisconnected(siteId));
        qDebug() << "socketDisconnected";
    }
    else {
        qDebug()<<"errore disconnessione socket";
    }

}

QMap<int, QString> SocketManager::getSiteIdUser() const
{
    return siteIdUser;
}

void SocketManager::setSiteIdUser(const QMap<int, QString> &value)
{
    siteIdUser = value;
}

