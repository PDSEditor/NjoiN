#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>

#include "message.h"

class Message;

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);

signals:

public slots:
    void updateDB(Message);
};

#endif // DATABASEMANAGER_H
