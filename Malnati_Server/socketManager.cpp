
#include "socketManager.h"
#include "QtWebSockets/qwebsocketserver.h"


SocketManager::SocketManager(QObject *parent) : QObject(parent),
    qWebSocketServer(new QWebSocketServer(QStringLiteral("Server Shared Editor"),
                                          QWebSocketServer::NonSecureMode, this))
    {

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
        QWebSocket *user = it.value();
        //serialize message in JSON
        user->sendTextMessage("Serialized message");
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

void SocketManager::processBinaryMessage(const QByteArray &data)
{
    QString dataStr = QString::fromStdString(data.toStdString());

    //Qui devo differenziare i byte tra action param e symbol
    /*
     * Messaggio:
     * il caratter 1 rappresenta l'azione
     *  Se 1 == I =73 (insert) o == D =68 (delete)
     *      allora il messaggio sarà composto nella seguente maniera "[?]-?-?"
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

    //enum Actions { I, D, R, C, L, T};

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

    int siteId = 0;       //vedere come gestire i siteId (probabilmente uno static int che si incrementa)
    clients.insert(siteId, socket);

    //successivamente comunicare al client il proprio siteId

}

void SocketManager::socketDisconnected()
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    //if (m_debug)
        //qDebug() << "socketDisconnected:" << pClient;

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
}

