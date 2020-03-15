#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include "QtWebSockets/qwebsocket.h"
#include "message.h"

class socketManager : public QObject
{
    Q_OBJECT    

public:
    explicit socketManager(QUrl &url, QObject *parent = nullptr);
     ~socketManager() override;

signals:
    void newMessage(Message* m);

public slots:
    void messageToServer (Message* m);
    void onConnected();
    void onTextMessageReceived(QString message);

private:
    QWebSocket webSocket;
    QUrl url;
};

#endif // SOCKETMANAGER_H
