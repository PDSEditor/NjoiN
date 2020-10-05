#include "databaseManager.h"
#include <iostream>
#include <QDebug>
#include <QCryptographicHash>

DatabaseManager::DatabaseManager()
{
    this->_instance =   std::make_unique<mongocxx::instance>();
    this->uri =         mongocxx::uri("mongodb://172.17.0.3:27017");
    this->uri =         mongocxx::uri("mongodb://localhost:27017");
    this->client =      mongocxx::client(this->uri);
    this->db =          mongocxx::database(this->client["mydb"]);
}

bool DatabaseManager::registerAccount(Account account, QString password, QByteArray &image){

    auto userCollection = (this->db)["user"];
    auto builder = bsoncxx::builder::stream::document{};

    QString hashpsw = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    std::vector<unsigned char> vector(image.begin(), image.end());
    bsoncxx::types::b_binary img {bsoncxx::binary_sub_type::k_binary,
                                 uint32_t(vector.size()),
                                 vector.data()
                                 };

    auto userToInsert = builder
            << "_id" << account.getUsername().toUtf8().constData()
            << "password" << hashpsw.toUtf8().toStdString()
            << "siteId" << account.getSiteId()
            << "image" << img
            << bsoncxx::builder::stream::finalize;

    auto view = userToInsert.view();

    try {
        userCollection.insert_one(view);
    } catch (mongocxx::bulk_write_exception& e) {
        qDebug() << e.what();
        qDebug() << "maybe duplicated?";
        return false;
    }

    return true;
}

Account DatabaseManager::getAccount(QString username){
    mongocxx::collection userCollection = (this->db)["user"];

    auto elementBuilder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value accountToRetrieve =
        elementBuilder << "_id" << username.toUtf8().constData()
                       << bsoncxx::builder::stream::finalize;
    bsoncxx::stdx::optional<bsoncxx::document::value> result;

    try{
        result = userCollection.find_one(accountToRetrieve.view());
    }catch (mongocxx::query_exception &e){
        qDebug() << "[ERROR][DatabaseManager::getAccount] find_one error, connection to db failed. Server should shutdown.";
        qDebug() << e.what();
        throw;
    }

    QList<QString> documentUris;
    QString string = QString::fromStdString(bsoncxx::to_json(result->view()));
    QJsonDocument documentJ = QJsonDocument::fromJson(string.toUtf8());

    for (auto i : documentJ["documentUris"].toArray()){
        documentUris.push_back(i.toString());
    }
    QString accountString = QString::fromStdString(bsoncxx::to_json(result->view()));
    QJsonDocument document = QJsonDocument::fromJson(accountString.toUtf8());
    auto array = document["image"].toString().toLatin1();
    Account account(document["_id"].toString(), document["siteId"].toInt(), array, documentUris);
    return account;
}

bool DatabaseManager::deleteAccount(QString _id){
    mongocxx::collection userCollection = (this->db)["user"];
    try {
        userCollection.delete_one(
                    bsoncxx::builder::stream::document{}
                    << "_id" << _id.toStdString()
                    << bsoncxx::builder::stream::finalize);
    } catch (mongocxx::bulk_write_exception& e) {
        qDebug() << e.what();
        return false;
    }

    return true;
}

bool DatabaseManager::checkAccountPsw(QString _id, QString password){

    mongocxx::collection userCollection = (this->db)["user"];

    QString hashpsw = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    bsoncxx::stdx::optional<bsoncxx::document::value> result;

    bsoncxx::document::value user =
            bsoncxx::builder::stream::document{}
            << "_id" << _id.toStdString()
            << bsoncxx::builder::stream::finalize;
    bsoncxx::document::view userView = user.view();
    try {
        result = userCollection.find_one(userView);
    } catch (mongocxx::query_exception& e) {
        qDebug() << "[ERROR][DatabaseManager::checkAccountPsw] find_one error, connection to db failed";
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

bool DatabaseManager::changePassword(QString _id, QString old_password, QString new_password){

    if(!this->checkAccountPsw(_id, old_password)) return false; //controllo che la vecchia password sia valida

    mongocxx::collection userCollection = (this->db)["user"];
    QString hashpsw = QCryptographicHash::hash(new_password.toUtf8(), QCryptographicHash::Sha256).toHex();


    bsoncxx::document::value user =
            bsoncxx::builder::stream::document{}
            << "_id" << _id.toStdString()
            << bsoncxx::builder::stream::finalize;

    bsoncxx::document::value newUser =
            bsoncxx::builder::stream::document{}
            << "$set" << bsoncxx::builder::stream::open_document
            << "password" << hashpsw.toUtf8().constData()
            << bsoncxx::builder::stream::close_document
            << bsoncxx::builder::stream::finalize;

    try {
        userCollection.update_one(user.view(), newUser.view());

    } catch (const mongocxx::bulk_write_exception &e) {
        qDebug() << "Error changing password, throws: ";
        qDebug() << e.what();
        return false;
    } catch (const mongocxx::logic_error &e) {
        qDebug() << "Error changing password, throws: ";
        qDebug() << e.what();
        return false;
    }
    return true;
}

bool DatabaseManager::changeImage(QString _id, QByteArray &image){
    //CONTROLLARE SE È AUTENTICATO?
    mongocxx::collection userCollection = (this->db)["user"];

    std::vector<unsigned char> vector(image.begin(), image.end());
    bsoncxx::types::b_binary img {bsoncxx::binary_sub_type::k_binary,
                                 uint32_t(vector.size()),
                                 vector.data()
                                 };

    bsoncxx::document::value user =
            bsoncxx::builder::stream::document{}
            << "_id" << _id.toStdString()
            << bsoncxx::builder::stream::finalize;

    bsoncxx::document::value newUser =
            bsoncxx::builder::stream::document{}
            << "$set" << bsoncxx::builder::stream::open_document
            << "image" << img
            << bsoncxx::builder::stream::close_document
            << bsoncxx::builder::stream::finalize;

    try {
        userCollection.update_one(user.view(), newUser.view());

    } catch (const mongocxx::bulk_write_exception &e) {
        qDebug() << "Error changing image, throws: ";
        qDebug() << e.what();
        return false;
    } catch (const mongocxx::logic_error &e) {
        qDebug() << "Error changing image, throws: ";
        qDebug() << e.what();
        return false;
    }

    return true;
}

bool DatabaseManager::insertSymbol(Message mes) {
    Symbol symbol = mes.getSymbol();
    QVector<QString> params = mes.getParams();
    QString documentId = params.at(0); //controllare se il documento esiste?
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
            << "value" << symbol.getValue().toLatin1()
            << "siteId" << symbol.getSiteId()
            << "counter" << symbol.getCounter()
            << "position" << array_builder
            << bsoncxx::builder::stream::finalize;

    bsoncxx::document::view view = symbolToInsert.view();
    try {
        symbolCollection.insert_one(view);
    } catch (mongocxx::bulk_write_exception& e) {
        qDebug() << "[ERROR][DatabaseManager::insertSymbol] insert_one error, connection to db failed.";
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
            builder << "value" << symbol.getValue().toLatin1()
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

bool DatabaseManager::insertDocument(SharedDocument document)
{
    mongocxx::collection documentCollection = (this->db)["document"];
    bsoncxx::stdx::optional<bsoncxx::document::value> result;
    auto builder = bsoncxx::builder::stream::document{};
    auto array_builder = bsoncxx::builder::basic::array{};

    for (QString i : document.getUserAllowed()){
        array_builder.append(i.toUtf8().constData());
    }

    QString id = (document.getName() + '_' + document.getCreator()).toUtf8().constData();

    bsoncxx::document::value documentToInsert = builder
            << "_id" << id
            << "documentName" << document.getName().toUtf8().constData()
            << "creator" << document.getCreator().toUtf8().constData()
//            << "isOpen" << document.getOpen() //da salvare nel db? NO
            << "userAllowed" << array_builder
            << bsoncxx::builder::stream::finalize;
    bsoncxx::document::view view = documentToInsert.view();

    try {
        documentCollection.insert_one(view);
    } catch (mongocxx::bulk_write_exception& e) {
        qDebug() << "[ERROR][DatabaseManager::insertDocument] insert_one error, maybe duplicated?";
        return false;
    }
    addAccountToDocument(id, document.getUserAllowed().at(0));
    return true;
}

SharedDocument DatabaseManager::getDocument(QString documentId){
    mongocxx::collection documents = this->db["document"];
    auto documentToRetrieve =
            bsoncxx::builder::stream::document{}
            << "_id" << documentId.toUtf8().constData()
            << bsoncxx::builder::stream::finalize;
    bsoncxx::stdx::optional<bsoncxx::document::value> result;
    try {
        result = documents.find_one(documentToRetrieve.view());
    } catch (mongocxx::query_exception &e) {
        qDebug() << e.what();
        throw;
    }

    if(result){
        QString string = QString::fromStdString(bsoncxx::to_json(result->view()));
        QJsonDocument document = QJsonDocument::fromJson(string.toUtf8());
        QList<QString> userAllowed;
        for (auto i : document["userAllowed"].toArray()){
            userAllowed.push_back(i.toString());
        }
        SharedDocument shared(document["documentName"].toString(), document["creator"].toString(), document["isOpen"].toBool(), userAllowed);
        return shared;
    }
    else throw;
}

QList<Symbol> DatabaseManager::retrieveSymbolsOfDocument(QString documentId)
{
    QList<Symbol> orderedSymbols;
    mongocxx::collection symbols = (this->db)["symbol"];

    auto elementBuilder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value symbolsToRetrieve =
        elementBuilder << "document_id" << documentId.toUtf8().constData()
                       << bsoncxx::builder::stream::finalize;
    bsoncxx::document::view symbolsToRetrieveView = symbolsToRetrieve.view();

    try{
        mongocxx::cursor resultIterator = symbols.find(symbolsToRetrieveView);
        for (auto elem : resultIterator) {
            QString symbol = QString::fromStdString(
                        bsoncxx::to_json(elem));
            QJsonDocument stringDocJSON = QJsonDocument::fromJson(symbol.toUtf8());
            QJsonObject symbolObjJson = stringDocJSON.object();
            Symbol symbolToInsert = Symbol::fromJson(symbolObjJson);
            orderedSymbols.push_back(symbolToInsert);
        }
    } catch (mongocxx::query_exception &e) {
        qDebug() << "[ERROR][DatabaseManager::retrieveSymbolsOfDocument] find error, connection to db failed";
        qDebug() << e.what();
        throw;
    }

    // TODO:test
    //It orders according to the order established in Char object,
    //so will be returned a vector in fractionalPosition ascending order
        std::sort(orderedSymbols.begin(), orderedSymbols.end());

        return orderedSymbols;
}

//QList<SharedDocument> DatabaseManager::getAllMyDocuments(QString username)
//{
//    mongocxx::collection documentCollection = this->db["document"];
//    auto elementBuilder = bsoncxx::builder::stream::document{};
//    bsoncxx::document::value documentsToRetrieve =
//            elementBuilder << "userAllowed" << siteId
//                           << bsoncxx::builder::stream::finalize;
//}

bool DatabaseManager::addAccountToDocument(QString documentId, QString username){
    mongocxx::collection documentCollection = this->db["document"];

    bsoncxx::document::value document =
            bsoncxx::builder::stream::document{}
            << "_id" << documentId.toUtf8().constData()
            << bsoncxx::builder::stream::finalize;

    bsoncxx::document::value newDocument =
            bsoncxx::builder::stream::document{}
            << "userAllowed"
            << bsoncxx::builder::stream::open_array << username.toUtf8().constData()
            << bsoncxx::builder::stream::close_array
            << bsoncxx::builder::stream::finalize;
    try {
        documentCollection.update_one(document.view(), newDocument.view());
        return true;
    } catch (mongocxx::bulk_write_exception &e) {
        qDebug() << e.what();
        return false;
    } catch (mongocxx::logic_error &e){
        qDebug() << e.what();
        return false;
    }
}

void DatabaseManager::changeDocumentName(QString documentId, QString newName){
    mongocxx::collection documentCollection = this->db["document"];

    bsoncxx::document::value document =
            bsoncxx::builder::stream::document{}
            << "_id" << documentId
            << bsoncxx::builder::stream::finalize;

    bsoncxx::document::value newDocument =
            bsoncxx::builder::stream::document{}
            << "$set" << bsoncxx::builder::stream::open_document
            << "documentName" << newName
            << bsoncxx::builder::stream::close_document
            << bsoncxx::builder::stream::finalize;

    try {
        documentCollection.update_one(document.view(), newDocument.view());
    } catch (const mongocxx::bulk_write_exception &e) {
        qDebug() << "Error changing document name in DB";
        throw;
    } catch (const mongocxx::logic_error &e){
        qDebug() << "Logic error changing document name in DB";
        throw;
    }
}

DatabaseManager::~DatabaseManager(){
    this->db.~database();
    this->client.~client();
    this->uri.~uri();
}
