#include "client.h"

Client::Client()
{


}
Client::~Client(){

    delete mw;
    if(closed==false){
        delete crdt;
    }
    delete sockm;

}

bool Client::Login()
{
    LoginWindow lw;
    Registration rw;
    AccountInterface ai;
    sockm = new socketManager(QUrl(QStringLiteral("ws://angelofloridia.ddns.net:8080")));
    //localhost:1234
    //93.51.19.124:8080
    //ws://angelofloridia.ddns.net:8080
//    sockm = new socketManager(QUrl(QStringLiteral("ws://localhost:1234")));
    mw = new MainWindow();

    connect(&lw,&LoginWindow::closeMw,mw,&MainWindow::closeMw);
    connect(&lw,&LoginWindow::openRw,&rw,&Registration::openRw);
    connect(&lw,&LoginWindow::closeRw,&rw,&Registration::closeRw);
    connect(&rw,&Registration::sendError,&lw,&LoginWindow::receiveErrorReg);
    connect(sockm,&socketManager::receiveRegistration,&lw,&LoginWindow::receiveRegistration);
    connect(&lw, &LoginWindow::sendMessage, sockm, &socketManager::messageToServer);
    connect(&rw, &Registration::sendMessage, sockm, &socketManager::messageToServer);
    connect(sockm, &socketManager::receivedLogin, &lw, &LoginWindow::receivedLogin);
    connect(sockm, &socketManager::loggedin,&lw, &LoginWindow::loggedin);
    connect(sockm, &socketManager::receivedInfoAccount, mw, &MainWindow::receivedInfoAccount);
    connect(sockm, &socketManager::receivedFile, mw, &MainWindow::receivedFile);
    connect(sockm, &socketManager::receivedURIerror, mw, &MainWindow::receiveURIerror);
    connect(sockm, &socketManager::setSiteId,&lw,&LoginWindow::receivedSiteId);
    connect(sockm, &socketManager::showUsers, mw, &MainWindow::showUsers);
    connect(sockm, &socketManager::receiveNewImage, mw, &MainWindow::receiveNewImageMW);
    connect(sockm, &socketManager::receiveNewPsw, mw, &MainWindow::receiveNewPswMW);

    lw.exec();

     //    /** TEST IMMAGINE! **/
     //    QString imgPath("/home/pepos/projects/progetto_malnati/Malnati_Client/images/cv_musk.png");
     //    QPixmap image(imgPath);

     //    //per averla sul json
     //    QJsonObject obj;
     //    QBuffer buffer;
     //    buffer.open(QIODevice::WriteOnly);
     //    image.save(&buffer, "PNG");
     //    auto const encoded = buffer.data().toBase64();
     //    obj.insert("image", QLatin1String(encoded));

     //    //per riaverla
     //    QByteArray l_image;
     //    l_image = obj["image"].toString().toLatin1(); //in base64

     //    QPixmap p;
     //    p.loadFromData(QByteArray::fromBase64(l_image), "PNG");

     //    /*****************************************/

    if(lw.getIsLogin()==true){
         mw->show();
         crdt=new Crdt();
         //connect(&webSocket, &QWebSocket::connected, this, &socketManager::onConnected);
         connect(mw, &MainWindow::newTextEdit, this, &Client::receive_textEdit);

//         connect(mw, &MainWindow::sendImage,sockm,&socketManager::receiveImage);
         connect(mw, &MainWindow::sendImage,sockm,&socketManager::messageToServer);
         connect(mw, &MainWindow::sendPwd, sockm, &socketManager::messageToServer);
         connect(mw,&MainWindow::sendMessage,sockm,&socketManager::binaryMessageToServer);
         connect(mw,&MainWindow::sendTextMessage,sockm,&socketManager::messageToServer);
         return true;

    }
    else{
       closed=true;
        return false;
    }
}

void Client::receive_textEdit(TextEdit *t,int s){
    this->textList.insert("prova1",t);
    this->crdt->setSiteId(s);
    t->setCrdt(this->crdt);
    t->setSocketM(Client::sockm);
    connect(sockm, &socketManager::newMessage, t, &TextEdit::receiveSymbol);
}



