#include "client.h"

Client::Client()
{
    LoginWindow lw;
    sockm=new socketManager(QUrl(QStringLiteral("ws://localhost:1234")));







    connect(&lw,&LoginWindow::sendMessage,sockm,&socketManager::binaryMessageToServer);

     lw.exec();
     mw = new MainWindow();

     if(lw.getIsLogin()==1){
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
    connect(sockm, &socketManager::newMessage, t, &TextEdit::reciveSymbol);
   // this->textList.insert("prova1",t);

}


