#include <QCoreApplication>

#include "server.h"
#include "socketManager.h"
#include "databaseManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server* server = new Server();
    SocketManager* socketMan = new SocketManager();
    DatabaseManager* dbMan = new DatabaseManager();

    QObject::connect(socketMan, &SocketManager::newMessage, server, &Server::dispatchMessage);
    QObject::connect(socketMan, &SocketManager::newMessage, dbMan, &DatabaseManager::updateDB);


    return a.exec();
}
