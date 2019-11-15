#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>

#include "message.h"

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    void insertInDB (Message* mes);
    void deleteFromDB (Message* mes);

signals:
    void sendFile(std::vector<Symbol> file);

public slots:
    void updateDB(Message* m);
    void createFile (std::string nome, int user);
    void retrieveFile(std::string nome, int user);
};

#endif // DATABASEMANAGER_H
