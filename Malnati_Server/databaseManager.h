#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>

#include "message.h"

class DatabaseManager : public QObject
{
    Q_OBJECT
    /*
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    void insertInDB (Message* mes);
    void deleteFromDB (Message* mes);

signals:
    void sendFile(std::vector<Symbol> file);

public slots:
    void updateDB(Message* m);
    void createFile (QString nome, int user);
    void retrieveFile(QString nome);
    */
};

#endif // DATABASEMANAGER_H


