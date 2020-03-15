<<<<<<< HEAD
#include "socketManager.h"
#include "QtWebSockets/qwebsocketserver.h"


SocketManager::SocketManager(QObject *parent) : QObject(parent),
    qWebSocketServer(new QWebSocketServer(QStringLiteral("Server Shared Editor"),
                                          QWebSocketServer::NonSecureMode, this))
    {
        port = 1234;

        if (qWebSocketServer->listen(QHostAddress::Any, port)) {   //ascolta su tutte le interfacce, posta scelta automaticamente


            qDebug() << "Server in ascolto sulla porta" << port;

            connect(qWebSocketServer, &QWebSocketServer::newConnection,
                    this, &SocketManager::onNewConnection);

            //connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &EchoServer::closed);

        }
}

SocketManager::~SocketManager()
{
     this->qWebSocketServer->close();
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

    qDebug()<<"Testo ricevuto: "<<message;

    Message *m = new Message();
    m->setAction(message);

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
    qDebug()<< "Rilevata nuova connessione";

    QWebSocket *socket = qWebSocketServer->nextPendingConnection();

    connect(socket, &QWebSocket::textMessageReceived, this, &SocketManager::processTextMessage);
      //connect(pSocket, &QWebSocket::binaryMessageReceived, this, &EchoServer::processBinaryMessage);
    connect(socket, &QWebSocket::disconnected, this, &SocketManager::socketDisconnected);

      int siteId = 0;       //vedere come gestire i siteId (probabilmente uno static int che si incrementa)
      clients.insert(siteId, socket);

      qDebug() << "socketConnected:" << siteId;

      //TODO: successivamente comunicare al client il proprio siteId

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

=======
#include "socketManager.h"
#include "QtWebSockets/qwebsocketserver.h"


SocketManager::SocketManager(QObject *parent) : QObject(parent),
    qWebSocketServer(new QWebSocketServer(QStringLiteral("Server Shared Editor"),
                                          QWebSocketServer::NonSecureMode, this))
    {

        if (qWebSocketServer->listen(QHostAddress::Any, 0)) {   //ascolta su tutte le interfacce, posta scelta automaticamente

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
    Message *m = new Message();         //crea il messaggio
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

    connect(socket, &QWebSocket::textMessageReceived, this, &SocketManager::processTextMessage);
      //connect(pSocket, &QWebSocket::binaryMessageReceived, this, &EchoServer::processBinaryMessage);
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

>>>>>>> 3e4cc998bb356e7d0d56f393ca13f3a6e9896e64
