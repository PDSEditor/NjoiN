<<<<<<< HEAD
#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include "QtWebSockets/qwebsocket.h"


#include "message.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)


class Message;

class SocketManager : public QObject
{
    Q_OBJECT

public:
    explicit SocketManager(QObject *parent = nullptr);
    void sendError (std::string); //da fare con le classi apposite per gli errori

    QMap<int, QWebSocket *> getClients() const;
    void setClients(const QMap<int, QWebSocket *> &value);

signals:
    void newMessage(Message* m);

public slots:
    void messageToUser (Message* m, int siteId);
    void fileToUser (std::vector<Symbol> file, int user);
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected();

private:
    QWebSocketServer* qWebSocketServer;
    QMap<int, QWebSocket *> clients;

};

#endif // SOCKETMANAGER_H
=======
<<<<<<< HEAD
#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QDebug>
#include "QtWebSockets/qwebsocket.h"

#include "message.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)

class Message;

class SocketManager : public QObject
{
    Q_OBJECT

public:
    explicit SocketManager(QObject *parent = nullptr);
     ~SocketManager() override;

    void sendError (std::string); //da fare con le classi apposite per gli errori

    QMap<int, QWebSocket *> getClients() const;
    void setClients(const QMap<int, QWebSocket *> &value);

signals:
    void newMessage(Message* m);

public slots:
    void messageToUser (Message* m, int siteId);
    void fileToUser (std::vector<Symbol> file, int user);
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected();

private:
    QWebSocketServer* qWebSocketServer;
    QMap<int, QWebSocket *> clients;
    int port;

};

#endif // SOCKETMANAGER_H
=======
#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include "QtWebSockets/qwebsocket.h"


#include "message.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)


class Message;

class SocketManager : public QObject
{
    Q_OBJECT

public:
    explicit SocketManager(QObject *parent = nullptr);
    void sendError (std::string); //da fare con le classi apposite per gli errori

    QMap<int, QWebSocket *> getClients() const;
    void setClients(const QMap<int, QWebSocket *> &value);

signals:
    void newMessage(Message* m);

public slots:
    void messageToUser (Message* m, int siteId);
    void fileToUser (std::vector<Symbol> file, int user);
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected();

private:
    QWebSocketServer* qWebSocketServer;
    QMap<int, QWebSocket *> clients;

};

#endif // SOCKETMANAGER_H
>>>>>>> 3e4cc998bb356e7d0d56f393ca13f3a6e9896e64
>>>>>>> d9ecd86c4d111839fb46b76d5c7d2117a31db1e7
