#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
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

#include <bsoncxx/json.hpp>


#include "message.h"
#include "account.h"
#include "sharedDocument.h"

/** PRIMO UTILIZZO **************************************************************************
 *
 * eseguire solo una volta:
 * echo "alias mongostart='sudo systemctl start mongod.service'" >> /home/mongovm/.bashrc
 * source /home/mongovm/.bashrc
 *
 * da ora in poi il db si può fare partire digitando semplicemente
 * 'mongostart' da terminale e fornendo la pswd di root per attivare il db
 * 'mongo' da terminale per entrare nel db;
 * ******************************************************************************************/


class DatabaseManager
{
private:
    std::unique_ptr<mongocxx::instance> _instance;
    mongocxx::database db;
    mongocxx::client client;
    mongocxx::uri uri;
    
public:
    DatabaseManager();
    /*** ACCOUNT ****/
    bool registerAccount(Account &account, QString password);
    bool deleteAccount  (QString _id);
    bool checkAccountPsw(QString _id, QString password);
    bool changePassword(QString _id, QString old_password, QString new_password);
    bool changeImage(QString _id, QByteArray &image);
    Account getAccount(QString username);
    /************/

    /** SYMBOL **/
    bool insertSymbol(Message &mes);
    bool deleteSymbol(Message &mes);
    /************/

    /** DOCUMENT **/
    bool insertDocument(SharedDocument &document);
    SharedDocument getDocument(QString documentId); /* cercare nella collezione il documento, e crearne uno con la lista dei simboli con retrieve simbols*/
    QList<Symbol> retrieveSymbolsOfDocument(QString documentId);
    QList<SharedDocument> getAllDocuments(); //todo: da fare
    QList<SharedDocument> getAllMyDocuments(QString username); //to test
    bool deleteDocument(QString documentId); //da fare
    QList<Account> getAccounts(QString documentName); //da fare
    QString getUri(QString documentName); //da fare
    void changeDocumentName(QString documentId, QString newName); //ricordarsi di catchare le exception se si usa
    /***********/

    /** URI **/
    bool addAccountToDocument(QString documentId, QString username);
    bool addDocumentToAccount(QString documentId, QString username);

    /*********/
    DatabaseManager(const DatabaseManager& other) = delete;
    DatabaseManager(const DatabaseManager&& other) = delete;
    ~DatabaseManager();
};

#endif // DATABASEMANAGER_H

