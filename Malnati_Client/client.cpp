#include "client.h"

Client::Client(MainWindow* mw)
{
    sockm=new socketManager(QUrl(QStringLiteral("ws://localhost:1234")));
    crdt=new Crdt();

        ptrmw = mw;


    //connect(&webSocket, &QWebSocket::connected, this, &socketManager::onConnected);
    connect(mw, &MainWindow::newTextEdit, this, &Client::receive_textEdit);
}

void Client::receive_textEdit(TextEdit *t){

    this->textList.insert("prova1",t);
    t->setCrdt(this->crdt);
    t->setSocketM(Client::sockm);
}

