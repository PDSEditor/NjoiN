#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QTextCodec>
#include "QtWebSockets/qwebsocket.h"


#include "message.h"

#define N_PORT 1234

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
    void processBinaryMessage(const QByteArray &data);
    void socketDisconnected();

private:
    QWebSocketServer* qWebSocketServer;
    QMap<int, QWebSocket *> clients;

};

#endif // SOCKETMANAGER_H
