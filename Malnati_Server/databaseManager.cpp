
#include "databaseManager.h"
#include <iostream>
#include <QDebug>



/*****************
 * da sistemare nel .h
 * per metterle nel .h si deve togliere il moc = meta-object compiler
 * https://doc.qt.io/archives/qt-4.8/moc.html
 * ***/

/*#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/exception/query_exception.hpp>
#include <mongocxx/exception/operation_exception.hpp>
#include <mongocxx/exception/bulk_write_exception.hpp>

*/
/*******************************************/

/*


DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    /**attributo dbmanager*/
    auto instance = new mongocxx::instance {};                         //this should be done only once.
    auto uri = new mongocxx::uri("mongodb://localhost:27017");
    auto client = new mongocxx::client( *(uri) );               //client istance connected to MongoDB server
    auto db = new mongocxx::database( (*client)["angelodb"] );

    mongocxx::collection countersCollection = (*db)["counter"];

        auto elementBuilder = bsoncxx::builder::stream::document{};
        QString nameDocument="Federico";
        bsoncxx::document::value elemToInsert =
            elementBuilder << "_id"                 << nameDocument.toUtf8().constData()
                           << "sequentialCounter"   << 1
                           << bsoncxx::builder::stream::finalize;
        bsoncxx::document::view elemToInsertView = elemToInsert.view();

        try {
            countersCollection.insert_one(elemToInsertView);
        } catch (mongocxx::operation_exception e) {
            qDebug() << "[ERROR][DatabaseManager::insertNewElemInCounterCollection] insert_one error, connection to db failed. Server should shutdown.";
            throw;
        }













//        mongocxx::instance inst{};
//        mongocxx::client conn{mongocxx::uri{}};

//        bsoncxx::builder::stream::document document{};

//        auto collection = conn["angelodb"]["fedecollection"];
//        document << "hello" << "shit!";

//        collection.insert_one(document.view());
//        auto cursor = collection.find({});

//        for (auto&& doc : cursor) {
//            std::cout << bsoncxx::to_json(doc) << std::endl;
//        }



}

void DatabaseManager::insertInDB(Message* mes) {

}

void DatabaseManager::deleteFromDB(Message* mes)
{

}

void DatabaseManager::updateDB(Message* m)
{

}

void DatabaseManager::createFile(QString nome, int user)
{

}

void DatabaseManager::retrieveFile(QString nome)
{

}
*/


