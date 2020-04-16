#include "databaseManager.h"
#include <iostream>
#include <QDebug>
#include <QCryptographicHash>

DatabaseManager::DatabaseManager()
{
//    this->instance = new mongocxx::instance{};
    //this->_instance = std::unique_ptr<mongocxx::instance>(new mongocxx::instance{});  //this should be done only once.

    //this->_instance = mongocxx::instance {};
    std::unique_ptr<mongocxx::instance> instance(new mongocxx::instance);
    this->_instance = std::move(instance);

    this->uri = mongocxx::uri("mongodb://localhost:27017");
    this->client = mongocxx::client(this->uri);

    this->db = mongocxx::database(this->client["mydb"]);
}

/*    mongocxx::collection countersCollection = db["counter"];

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
}*/

int DatabaseManager::registerUser(QString _id, QString password){

    mongocxx::collection userCollection = (this->db)["user"];

    auto builder = bsoncxx::builder::stream::document{};

    QString hashpsw = QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Sha256);

    bsoncxx::document::value userToInsert = builder
            << "_id" << _id.toStdString()
            << "password" << hashpsw.toStdString()
            /*<< "array"    << bsoncxx::builder::stream::open_array
                << "banana" << "mela" << "pera"
            << bsoncxx::builder::stream::close_array
            << "info" << bsoncxx::builder::stream::open_document
                << "sesso" << "maschio"
                << "eta"   << 23
            << bsoncxx::builder::stream::close_document*/
            << bsoncxx::builder::stream::finalize;

    bsoncxx::document::view view = userToInsert.view();

    /*bsoncxx::document::element element = view["username"];

    if(element.type() != bsoncxx::type::k_utf8){
        return -1;
    }
    std::string name = element.get_utf8().value.to_string();*/

    try {
        userCollection.insert_one(view);
    } catch (mongocxx::bulk_write_exception& e) {
        return -1;
    }

    return 0;
}

int DatabaseManager::deleteUser(QString _id){
    mongocxx::collection userCollection = (this->db)["user"];
    try {
        userCollection.delete_one(
                    bsoncxx::builder::stream::document{}
                    << "_id" << _id.toStdString()
                    << bsoncxx::builder::stream::finalize);
    } catch (mongocxx::bulk_write_exception& e) {
        return -1;
    }

    return 0;
}

int DatabaseManager::checkUserPsw(QString _id, QString password){

    mongocxx::collection userCollection = (this->db)["user"];

    QString hashpsw = QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Sha256);

    bsoncxx::stdx::optional<bsoncxx::document::value> result;

    bsoncxx::document::value user =
            bsoncxx::builder::stream::document{}
            << "_id" << _id.toStdString()
            << bsoncxx::builder::stream::finalize;
    bsoncxx::document::view userView = user.view();
    try {
        result = userCollection.find_one(userView);
    } catch (mongocxx::query_exception& e) {
        return -1;
        throw;
    }

    if(result){
        bsoncxx::document::view a = (*result).view();
        bsoncxx::document::element element = a["password"];
        std::string b = element.get_utf8().value.to_string();

        if(b==hashpsw.toStdString())
            return 0;
        else return -1;
    }
    return 0;
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

DatabaseManager::~DatabaseManager(){
    this->db.~database();
    this->client.~client();
    this->uri.~uri();
    //this->_instance.~instance();
}
