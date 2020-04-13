#include "client.h"

Client::Client()
{
    sockm=new socketManager(QUrl(QStringLiteral("ws://localhost:1234")));
    Crdt crdt;
     LoginWindow lw;

     lw.exec();
        MainWindow mw;
     if(lw.getIsLogin()==1){

         ptrmw = &mw;
         mw.show();
     }

    //connect(&webSocket, &QWebSocket::connected, this, &socketManager::onConnected);
    connect(&mw, &MainWindow::newTextEdit, this, &Client::receive_textEdit);
}

void Client::receive_textEdit(TextEdit *t){
    this->textList.insert("prova1",t);
}
