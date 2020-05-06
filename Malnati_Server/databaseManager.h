#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

#include <mongocxx/exception/query_exception.hpp>
#include <mongocxx/exception/operation_exception.hpp>
#include <mongocxx/exception/bulk_write_exception.hpp>

#include <memory>
#include <functional>

#include <message.h>

class DatabaseManager
{
private:
    std::unique_ptr<mongocxx::instance> _instance;
    mongocxx::database db;
    mongocxx::client client;
    mongocxx::uri uri;
    
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    /*** USER ****/
    bool registerUser(QString _id, QString password);
    bool deleteUser  (QString _id);
    bool checkUserPsw(QString _id, QString password);
    /************/

    bool insertInDB(Message mes);
    void deleteFromDB(Message mes);
    void updateDB(Message m);
    void createFile(QString nome, int user);
    void retrieveFile(QString nome);

    ~DatabaseManager();
};

#endif // DATABASEMANAGER_H
