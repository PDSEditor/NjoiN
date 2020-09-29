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
#include <mongocxx/exception/logic_error.hpp>

#include <memory>
#include <functional>

#include "message.h"
#include "account.h"
#include "sharedDocument.h"

class DatabaseManager
{
private:
    std::unique_ptr<mongocxx::instance> _instance;
    mongocxx::database db;
    mongocxx::client client;
    mongocxx::uri uri;
    
public:
    DatabaseManager();
    /*** USER ****/
    bool registerAccount(Account account, QString password, QByteArray &image);
    bool deleteAccount  (QString _id);
    bool checkAccountPsw(QString _id, QString password);
    bool changePassword(QString _id, QString old_password, QString new_password);
    bool changeImage(QString _id, QByteArray &image);
    Account getAccount(QString username);
    /************/

    /** SYMBOL **/
    bool insertSymbol(Message mes);
    bool deleteSymbol(Message mes);
    /************/

    /** DOCUMENT **/
    bool insertDocument(SharedDocument document);
    SharedDocument getDocument(QString documentName); /* cercare nella collezione il documento, e crearne uno con la lista dei simboli con retrieve simbols*/
    QList<Symbol> retrieveSymbolsOfDocument(QString documentName);
    QList<SharedDocument> getAllDocuments(); //todo: da fare
    QList<Account> getAccounts(QString documentName); //da fare
    
//    bool deleteDocument(SharedDocument document);
    /***********/

    /** URI **/

    /*********/
    DatabaseManager(const DatabaseManager& other) = delete;
    DatabaseManager(const DatabaseManager&& other) = delete;
    ~DatabaseManager();
};

#endif // DATABASEMANAGER_H

