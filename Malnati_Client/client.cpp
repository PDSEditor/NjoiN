#include "client.h"

Client::Client()
{
    LoginWindow lw;
    sockm=new socketManager(QUrl(QStringLiteral("ws://localhost:1234")));










     lw.exec();
         mw = new MainWindow();
     if(lw.getIsLogin()==1){
         mw->show();
     }
    crdt=new Crdt();
    //connect(&webSocket, &QWebSocket::connected, this, &socketManager::onConnected);
    connect(mw, &MainWindow::newTextEdit, this, &Client::receive_textEdit);
}
Client::~Client(){
    delete mw;
    delete crdt;

}
void Client::receive_textEdit(TextEdit *t){


    this->textList.insert("prova1",t);
    t->setCrdt(this->crdt);
    t->setSocketM(Client::sockm);

   // this->textList.insert("prova1",t);

}

