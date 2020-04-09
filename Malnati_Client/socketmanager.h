#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include "QtWebSockets/qwebsocket.h"
#include "message.h"
#include <QDebug>
#include <string>
#include "string.h"
#include <QDataStream>


class socketManager : public QObject
{
    Q_OBJECT    

public:
    explicit socketManager(const QUrl &url, QObject *parent = nullptr);
     ~socketManager() override;

signals:
    void newMessage(Message* m);

public slots:
    void messageToServer (Message* m);
    void binaryMessageToServer (Message* m); //Send a message from client to server
    void onConnected();
    void onTextMessageReceived(QString message);
    void onBinaryMessageReceived(QByteArray bytemex); //Received binary message from server and emit a signal

private:
    QWebSocket webSocket;
    QUrl url;
};

#endif // SOCKETMANAGER_H
