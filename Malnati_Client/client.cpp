#include "client.h"

Client::Client()
{
    LoginWindow lw;
    sockm = new socketManager(QUrl(QStringLiteral("ws://localhost:1234")));
    mw = new MainWindow();
    //connect(&lw, &LoginWindow::sendMessage, sockm, &socketManager::binaryMessageToServer);
    connect(&lw, &LoginWindow::sendMessage, sockm, &socketManager::messageToServer);
    connect(sockm, &socketManager::receivedLogin, &lw, &LoginWindow::receivedLogin);
    connect(sockm, &socketManager::receivedInfoAccount, mw, &MainWindow::receivedInfoAccount);
    connect(sockm, &socketManager::receivedFile, mw, &MainWindow::receivedFile);
    connect(sockm, &socketManager::receivedURIerror, mw, &MainWindow::receiveURIerror);
    connect(sockm,&socketManager::setSiteId,&lw,&LoginWindow::receivedSiteId);

    /** TEST IMMAGINE! **/
    QString imgPath("/home/pepos/projects/progetto_malnati/Malnati_Client/images/cv_musk.png");
    QPixmap image(imgPath);

    //per averla sul json
    QJsonObject obj;
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    auto const encoded = buffer.data().toBase64();
    obj.insert("image", QLatin1String(encoded));

    //per riaverla



    /*****************************************/


    lw.exec();

    if(lw.getIsLogin()==true){
         mw->show();
    }

    crdt=new Crdt();
    //connect(&webSocket, &QWebSocket::connected, this, &socketManager::onConnected);
    connect(mw, &MainWindow::newTextEdit, this, &Client::receive_textEdit);

    connect(mw, &MainWindow::sendImage,sockm,&socketManager::receiveImage);
    connect(mw,&MainWindow::sendMessage,sockm,&socketManager::binaryMessageToServer);
    connect(mw,&MainWindow::sendTextMessage,sockm,&socketManager::messageToServer);
}
Client::~Client(){
    delete mw;
    delete crdt;
    delete sockm;

}

void Client::receive_textEdit(TextEdit *t){


    this->textList.insert("prova1",t);
    t->setCrdt(this->crdt);
    t->setSocketM(Client::sockm);
    connect(sockm, &socketManager::newMessage, t, &TextEdit::reciveSymbol);
   // this->textList.insert("prova1",t);

}


