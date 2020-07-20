
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

void SocketManager::messageToUser( Message* m, int siteId) {

   //TODO: invia il singolo messaggio ai vari client

    auto it = this->clients.find(siteId);
    if (it != clients.end()) {
        qDebug()<<"Invio site id al client n "<< siteId;
        QWebSocket *user = it.value();
        //serialize message in JSON
        binaryMessageToUser(m, siteId);
        //user->sendBinaryMessage("Serialized message");
    }
}

void SocketManager::binaryMessageToUser(Message *m, int siteId)
{
    int tmp;
    QByteArray bytemex;
    QChar action = m->getAction();
    Symbol symbol = m->getSymbol();
    QVector<QString> params = m->getParams();


    if(action==("I")||action==("D")){
        if(action==("I")){
            bytemex.append('I');
        }
        else{
            bytemex.append('D');
        }

        bytemex.append('{');
        for(unsigned long long i=0;i<symbol.getPosizione().size();i++){
            tmp=(symbol.getPosizione().at(i));

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
        bytemex.append(symbol.getValue());

    }
    else if(action==('C')||action==('R')){
        if(action==('C')){
            bytemex.append('C');
        }
        else{
            bytemex.append('R');
        }
        bytemex.append(params.at(0));

    }
    else if(action=='L'){
        bytemex.append('L');
        tmp=params.at(0).length();
        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
        bytemex.append(params.at(0));
        tmp=params.at(1).length();
        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
        bytemex.append(params.at(1));
    }
    else if (action == 'S') {
        bytemex.append('S');
        tmp=params.at(0).length();
        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
        bytemex.append(params.at(0));
    }

    //qDebug()<<'lunghezza array di byte'<<bytemex.size();
    //webSocket.sendBinaryMessage( bytemex);
    auto it = this->clients.find(siteId);
    if (it != clients.end()) {
        QWebSocket *user = it.value();
        user->sendBinaryMessage(bytemex);
    }
}

void SocketManager::fileToUser(std::vector<Symbol> file, int user)
{

}

void SocketManager::processTextMessage(QString message)
{
    //deserialize JSON
    //QWebSocket *client = qobject_cast<QWebSocket *>(sender());    probabilmente non serve, il sender è già identificato tramite SiteId
    qDebug()<<message;
    Message *m = new Message();         //crea il messaggio
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

    /*
    QString dataStr = QString::fromStdString(data.toStdString());

    QChar action = dataStr[0];

    ushort act = action.unicode();

    Message *m = new Message(action);

    switch(act){
    case (73| 68):
    {
        //Prima costruisco il vettore posizione
        std::vector<int> posizione;
        int n;
        int i =1;
        if (dataStr[i].unicode() == '[') {
            i++;
            while(dataStr[i].unicode() != ']') {
                if (dataStr[i].unicode() == ',') {
                    posizione.push_back(n);
                    n=0;
                }
                else {
                    n = (n*10) + dataStr[i].unicode()-48;
                }
                i++;
            }
        }
        else {
            sendError ("Errore di formattazione nel messaggio");
        }

        i +=2 ; //sono passato dalla parentesi quadra all'elemento dopo il "-"
        //prendo il siteId
        int siteId=0;
        while (dataStr[i].unicode() != '-') {
            siteId = siteId*10 + dataStr[i].unicode()-48;
            i++;
        }
        i++;
        //prendo il counter
        int counter = 0;
        auto it = dataStr.begin() + i;
        while (it != dataStr.end()){
            counter = counter *10 + it->unicode()-48;
        }

        Symbol *symbol = new Symbol(posizione, siteId, counter);

        m->setSymbol(symbol);

        break;
    }

    case (82|67) :
    {
        QString nomeFile;
        int lungNome = dataStr[1].unicode()-48;
        for (int i = 2; i < lungNome+2; i++) {
            QChar l = dataStr[i];
            nomeFile.push_back(l);
        }


        QVector<QString> params;
        params.push_back(nomeFile);
        m->setParams(params);
        break;
    }

    case (76) :
    {
        QString user;
        int lungUser = dataStr[1].unicode()-48;
        for (int i = 2; i < lungUser+2; i++) {
            QChar l = dataStr[i];
            user.push_back(l);
        }

        QString pw;
        int lungPw = dataStr[lungUser+2].unicode()-48;
        for (int i = 2; i < lungPw+2; i++) {
            QChar l = dataStr[i];
            pw.push_back(l);
        }


        QVector<QString> params;
        params.push_back(user);
        params.push_back(pw);
        m->setParams(params);
        break;
    }

    case (84): {
        qDebug()<<dataStr;
        break;
    }

    default:
        sendError("Generic Error");
    }



    emit newMessage(m);
    */

    QByteArray c;
    int tmp;
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
        symbol.setPosizione(vtmp);
        symbol.setSiteId((int)bytemex.at(i++));
        c.clear();
        c.append(bytemex.mid(i,4));
        memcpy(&tmp,c,4);
        symbol.setCounter(tmp);
        i+=4;
        symbol.setValue(bytemex.at(i));
    }
    else if(bytemex.at(0)=='C'||bytemex.at(0)=='R'){
        if(bytemex.at(0)=='C')
            action='C';
        else
            action='R';
        params.push_back(bytemex.right(bytemex.length()-1));
    }
    else if(bytemex.at(0)=='L'){
        action='L';
        c.clear();
        c.append(bytemex.mid(1,4));
        memcpy(&tmp,c,4);
        params.push_back(bytemex.mid(5,tmp));
        c.clear();
        c.append(bytemex.mid(tmp+5,4));
        memcpy(&tmp,c,4);
        params.push_back(bytemex.right(tmp));
    }

    Message *m = new Message;
    m->setAction(action);
    m->setParams(params);
    m->setSymbol(symbol);

    m->debugPrint();
    //
    this->binaryMessageToUser(m,0);
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

    //versione text message era funzionante
    connect(socket, &QWebSocket::textMessageReceived, this, &SocketManager::processTextMessage);
    connect(socket, &QWebSocket::binaryMessageReceived, this, &SocketManager::processBinaryMessage);
    connect(socket, &QWebSocket::disconnected, this, &SocketManager::socketDisconnected);

    clients.insert(SocketManager::siteId, socket);


    //successivamente comunicare al client il proprio siteId
    Message m;


    m.setAction('S');
    QString s = QString::number(SocketManager::siteId);
    m.setParams({s});
    messageToUser(&m,SocketManager::siteId);

    emit newAccountOnline(SocketManager::siteId);
    SocketManager::siteId++;



}

void SocketManager::socketDisconnected()
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    //if (m_debug)
        qDebug() << "socketDisconnected";

    if (client) {
        auto it = clients.begin();
        while (it!= clients.end()) {
            if(it.value()==client)
                it = clients.erase(it);
            else
                it++;
        }
        client->deleteLater();
    }

    //emetti segnale per rimuovere da onlineAccounts

}

