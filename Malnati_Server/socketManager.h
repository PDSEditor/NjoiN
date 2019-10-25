#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>

#include "message.h"

class Message;

class SocketManager : public QObject
{
    Q_OBJECT
public:
    explicit SocketManager(QObject *parent = nullptr);

signals:
    void newMessage(Message);

public slots:
};

#endif // SOCKETMANAGER_H
