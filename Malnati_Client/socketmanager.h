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
    void newMessage(Message *m);
    void receivedLogin(bool resp);
    void receivedInfoAccount(Message&);
    void setSiteId(int);
    void receivedFile();

public slots:
    void receiveImage(QByteArray im);
    void onConnected();
    void onTextMessageReceived(QString message);
    void onBinaryMessageReceived(QByteArray bytemex); //Received binary message from server and emit a signal
    void binaryMessageToServer (Message* m);
    void messageToServer (Message* m);
    //


//
public:



    bool getServerOn() const;
    void setServerOn(bool value);

private:
    QWebSocket webSocket;
    QUrl url;
    int i=100;
    int siteId;
    bool serverOn;
};

#endif // SOCKETMANAGER_H
