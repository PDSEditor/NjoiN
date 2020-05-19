#include "databaseManager.h"
#include <iostream>
#include <QDebug>
#include <QCryptographicHash>

//DatabaseManager::DatabaseManager(QObject* parent)
DatabaseManager::DatabaseManager()
{
//    std::unique_ptr<mongocxx::instance> instance(new mongocxx::instance);
//    this->_instance = std::move(instance);
    this->_instance =   std::make_unique<mongocxx::instance>();
    this->uri =         mongocxx::uri("mongodb://172.17.0.3:27017");
    this->uri =         mongocxx::uri("mongodb://localhost:27017");
    this->client =      mongocxx::client(this->uri);
    this->db =          mongocxx::database(this->client["mydb"]);
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

bool DatabaseManager::registerUser(QString _id, QString password){

    auto userCollection = (this->db)["user"];
//    mongocxx::collection userCollection = (this->db)["user"];

    auto builder = bsoncxx::builder::stream::document{};

    QString hashpsw = QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Sha256);

//    bsoncxx::document::value userToInsert = builder
    auto userToInsert = builder
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

//    bsoncxx::document::view view = userToInsert.view();
    auto view = userToInsert.view();

    /*bsoncxx::document::element element = view["username"];

    if(element.type() != bsoncxx::type::k_utf8){
        return -1;
    }
    std::string name = element.get_utf8().value.to_string();*/

    try {
        userCollection.insert_one(view);
    } catch (mongocxx::bulk_write_exception& e) {
        return false;
    }

    return true;
}

bool DatabaseManager::deleteUser(QString _id){
    mongocxx::collection userCollection = (this->db)["user"];
    try {
        userCollection.delete_one(
                    bsoncxx::builder::stream::document{}
                    << "_id" << _id.toStdString()
                    << bsoncxx::builder::stream::finalize);
    } catch (mongocxx::bulk_write_exception& e) {
        return false;
    }

    return true;
}

bool DatabaseManager::checkUserPsw(QString _id, QString password){

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
        return false;
    }

    if(result){
        bsoncxx::document::view a = (*result).view();
        bsoncxx::document::element element = a["password"];
        std::string b = element.get_utf8().value.to_string();

        if(b==hashpsw.toStdString())
            return true;
        else return false;
    }
    else return false;
}

bool DatabaseManager::insertInDB(Message mes) {
    Symbol symbol = mes.getSymbol();
    mongocxx::collection symbolCollection = (this->db)["symbol"];
    bsoncxx::stdx::optional<bsoncxx::document::value> result;
    auto builder = bsoncxx::builder::stream::document{};

    bsoncxx::document::value symbolToInsert = builder
            /*<< "_id" << ? */
            << "value" << symbol.getValue()
            << "siteId" << symbol.getSiteId()
            << "counter" << symbol.getCounter()
            << "posizione" << symbol.getPosizione().at(0)
               /****** DA METTERE TUTTO IL VETTORE NON SOLO 0 **************/
            << bsoncxx::builder::stream::finalize;

    bsoncxx::document::view view = symbolToInsert.view();
    try {
        symbolCollection.insert_one(view);
    } catch (mongocxx::bulk_write_exception& e) {
        return false;
    }
    return true;

}

void DatabaseManager::deleteFromDB(Message mes)
{

}

void DatabaseManager::updateDB(Message m)
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
}
