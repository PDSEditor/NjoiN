#include "databaseManager.h"
#include <iostream>
#include <QDebug>
#include <QCryptographicHash>

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

bool DatabaseManager::registerUser(Account account, QString password){

    auto userCollection = (this->db)["user"];
    auto builder = bsoncxx::builder::stream::document{};

    QString hashpsw = QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Sha256);

    auto userToInsert = builder
            << "_id" << account.getUsername().toUtf8().constData()
            << "password" << hashpsw.toStdString()
            << "siteId" << account.getSiteId()
            << bsoncxx::builder::stream::finalize;

    auto view = userToInsert.view();

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
        qDebug() << "[ERROR][DatabaseManager::checkUserPsw] find_one error, connection to db failed. Server should shutdown.";
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

Account DatabaseManager::getAccount(QString username){
    mongocxx::collection userCollection = (this->db)["user"];

    auto elementBuilder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value accountToRetrieve =
        elementBuilder << "_id" << username.toUtf8().constData()
                       << bsoncxx::builder::stream::finalize;
    bsoncxx::document::view accountToRetrieveView = accountToRetrieve.view();
    bsoncxx::stdx::optional<bsoncxx::document::value> result;
    Account account;
    try{
        result = userCollection.find_one(accountToRetrieveView);
        if(result){
            auto a = (*result).view();
            auto siteId = a["siteId"];
            account.setUsername(username);
            account.setSiteId(siteId.get_int32());
        }
    } catch (mongocxx::query_exception &e){
        qDebug() << "[ERROR][DatabaseManager::getAccount] find_one error, connection to db failed. Server should shutdown.";
    }
    return account;
}

bool DatabaseManager::insertSymbol(Message mes) {
    Symbol symbol = mes.getSymbol();
    QVector<QString> params = mes.getParams();
    QString documentId = params.at(0);
    mongocxx::collection symbolCollection = (this->db)["symbol"];
    bsoncxx::stdx::optional<bsoncxx::document::value> result;
    auto builder = bsoncxx::builder::stream::document{};

    auto array_builder = bsoncxx::builder::basic::array{};

    for (int i : symbol.getPosition()){
        array_builder.append(i);
    }

    bsoncxx::document::value symbolToInsert = builder
            /*<< "_id" << ? */
            << "document_id" << documentId.toUtf8().constData()
            << "value" << symbol.getValue()
            << "siteId" << symbol.getSiteId()
            << "counter" << symbol.getCounter()
            << "position" << array_builder
            << bsoncxx::builder::stream::finalize;

    bsoncxx::document::view view = symbolToInsert.view();
    try {
        symbolCollection.insert_one(view);
    } catch (mongocxx::bulk_write_exception& e) {
        qDebug() << "[ERROR][DatabaseManager::insertSymbol] insert_one error, connection to db failed. Server should shutdown.";
        return false;
    }
    return true;

}

bool DatabaseManager::deleteSymbol(Message mes)
{
    Symbol symbol = mes.getSymbol();
    QVector<QString> params = mes.getParams();
    QString documentName = params.at(0);
    mongocxx::collection symbolCollection = (this->db)["symbol"];
    bsoncxx::stdx::optional<bsoncxx::document::value> result;
    auto builder = bsoncxx::builder::stream::document{};

    bsoncxx::document::value symbolToDelete =
            builder << "value" << symbol.getValue()
                    << "siteId" << symbol.getSiteId()
                    << "counter" << symbol.getCounter()
                    << "documentName" << documentName.toUtf8().constData()
                    << bsoncxx::builder::stream::finalize;
    bsoncxx::document::view view = symbolToDelete.view();
    try {
        symbolCollection.delete_one(view);
    } catch (mongocxx::exception &e) {
        qDebug() << "[ERROR][DatabaseManager::deleteFromDB] delete_one error, connection to db failed. Server should shutdown.";
        return false;
    }
    return true;
}

bool DatabaseManager::createDocument(SharedDocument document)
{
    mongocxx::collection documentCollection = (this->db)["document"];
    bsoncxx::stdx::optional<bsoncxx::document::value> result;
    auto builder = bsoncxx::builder::stream::document{};
    auto array_builder = bsoncxx::builder::basic::array{};

    for (int i : document.getUserAllowed()){
        array_builder.append(i);
    }

    bsoncxx::document::value documentToInsert = builder
            << "_id" << (document.getName() + '_' + QString::number(document.getCreator())).toUtf8().constData()
            << "documentName" << document.getName().toUtf8().constData()
            << "creator" << document.getCreator()
            << "isOpen" << document.getOpen()
            << "userAllowed" << array_builder
            << bsoncxx::builder::stream::finalize;

    bsoncxx::document::view view = documentToInsert.view();
    try {
        documentCollection.insert_one(view);
    } catch (mongocxx::bulk_write_exception& e) {
        qDebug() << "[ERROR][DatabaseManager::insertDocument] insert_one error, connection to db failed. Server should shutdown.";
        return false;
    }
    return true;
}

QList<Symbol> DatabaseManager::retrieveFile(QString documentName)
{
    QList<Symbol> orderedSymbols;
    mongocxx::collection symbols = (this->db)["symbol"];

    auto elementBuilder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value symbolsToRetrieve =
        elementBuilder << "documentName" << documentName.toUtf8().constData()
                       << bsoncxx::builder::stream::finalize;
    bsoncxx::document::view symbolsToRetrieveView = symbolsToRetrieve.view();

    // notice that the only instruction that should be included in try-catch
    // is insertCollection.find, but we do in this way because of scope problems.
    // Despite everything, we are sure that this is the only method that can raise
    // the below type of exception in catch().
    try{
        mongocxx::cursor resultIterator = symbols.find(symbolsToRetrieveView);
        //(.1)Save them in local before ordering
        for (auto elem : resultIterator) {
            QString symbol = QString::fromStdString(bsoncxx::to_json(elem));
            QJsonDocument stringDocJSON = QJsonDocument::fromJson(symbol.toUtf8());
            QJsonObject symbolObjJson = stringDocJSON.object();
            Symbol symbolToInsert = Symbol::fromJson(symbolObjJson);
            orderedSymbols.push_back(symbolToInsert);
        }
    } catch (mongocxx::query_exception) {
        qDebug() << "[ERROR][DatabaseManager::getAllInserts] find error, connection to db failed. Server should shutdown.";
        throw;
    }

    //(.2)Now ordering.
    //It orders according to the order established in Char object,
    //so will be returned a vector in fractionalPosition ascending order
//        std::sort(orderedSymbols.begin(), orderedSymbols.end());

       return orderedSymbols;


}

DatabaseManager::~DatabaseManager(){
    this->db.~database();
    this->client.~client();
    this->uri.~uri();
}
