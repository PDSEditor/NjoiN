#include "socketmanager.h"

socketManager::socketManager(QUrl &url, QObject *parent) : QObject(parent)
{
    url = *(new QUrl("localhost:1234"));
    connect(&webSocket, &QWebSocket::connected, this, &socketManager::onConnected);
    //connect(webSocket, &QWebSocket::disconnected, this, &socketManager::closed);
    webSocket.open(QUrl(url));

}

socketManager::~socketManager()
{
    this->webSocket.close();
}

void socketManager::messageToServer(Message *m)
{

}

void socketManager::onConnected()
{
    connect(&webSocket, &QWebSocket::textMessageReceived,
               this, &socketManager::onTextMessageReceived);
    //webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
    qDebug() << "socket Connected";

}

void socketManager::onTextMessageReceived(QString message)
{

}
