#include "client.h"

Client::Client()
{
    LoginWindow lw;
    sockm=new socketManager(QUrl(QStringLiteral("ws://localhost:1234")));

    connect(&lw,&LoginWindow::sendMessage,sockm,&socketManager::binaryMessageToServer);
    connect(sockm,&socketManager::receivedLogin,&lw,&LoginWindow::receivedLogin);
    connect(sockm,&socketManager::receivedInfoAccount,mw,&MainWindow::receivedInfoAccount);

     lw.exec();


     if(lw.getIsLogin()==1){
         mw = new MainWindow();
         mw->show();
     }
    crdt=new Crdt();
    //connect(&webSocket, &QWebSocket::connected, this, &socketManager::onConnected);
    connect(mw, &MainWindow::newTextEdit, this, &Client::receive_textEdit);

    connect(mw, &MainWindow::sendImage,sockm,&socketManager::receiveImage);
    connect(mw,&MainWindow::sendMessage,sockm,&socketManager::binaryMessageToServer);
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
    connect(sockm, &socketManager::newMessage, t, &TextEdit::receiveSymbol);
   // this->textList.insert("prova1",t);

}


