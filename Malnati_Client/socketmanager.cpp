#include "socketmanager.h"

socketManager::socketManager(const QUrl &url,  QObject *parent) : QObject(parent)
{
    //url = *(new QUrl("localhost:1234"));
    connect(&webSocket, &QWebSocket::connected, this, &socketManager::onConnected);
    //connect(webSocket, &QWebSocket::disconnected, this, &socketManager::closed);
    //webSocket= new QWebSocket();
    webSocket.open(QUrl(url));
    //qDebug()<<webSocket.isValid();

    siteId = 0;
}

socketManager::~socketManager()
{
    this->webSocket.close();
}

void socketManager::messageToServer(Message *m)
{
    //QString tmp = m->getAction();
    //webSocket.sendTextMessage(tmp);

    //qDebug()<<"Testo inviato: sia m diu ";
}

void socketManager::receiveImage(QByteArray im){
    int i=0;
}


//Send a message from client to server
void socketManager::binaryMessageToServer(Message *m)
{
    //qDebug()<<"Testo ricevuto: ";

    int tmp;
    QChar tmpc;
    QByteArray bytemex;
    QChar action = m->getAction();




    Symbol symbol = m->getSymbol();
    symbol.setSiteId(socketManager::siteId);



    if(action==('I')||action==("D")){
        if(action==('I')){
            bytemex.append('I');
        }
        else{
            bytemex.append('D');
        }

        bytemex.append('{');
        for(unsigned long long i=0;i<symbol.getPosizione().size();i++){
            tmp=(symbol.getPosizione().at(i));

            for(int p=0;p<4;p++){
                bytemex.append(tmp >> (p * 8));
            }
        }

        bytemex.append('}');
        bytemex.append(symbol.getSiteId());//dimensione massima
        tmp=(symbol.getCounter());

        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
        tmp=(symbol.getCounter());
        tmpc=symbol.getValue();
        bytemex.append(tmpc.cell());
        bytemex.append(tmpc.row());


        //inserimento info testo

        bytemex.append(symbol.getBold());
        bytemex.append(symbol.getUnderln());
        bytemex.append(symbol.getItalic());
        tmp=symbol.getSize();
        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
        tmp=siteId;
        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
        bytemex.append(symbol.getFamily());


    }
    else if(action==('C')||action==('R')){
        QVector<QString> params = m->getParams();
        if(action==('C')){
            bytemex.append('C');
        }
        else{
            bytemex.append('R');
        }
        tmp=siteId;
        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
        bytemex.append(params.at(0));

    }
    else if(action=='L'){
        QVector<QString> params = m->getParams();
        bytemex.append('L');
        tmp=siteId;
        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
        tmp=params.at(0).length();
        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
        bytemex.append(params.at(0));
        tmp=params.at(1).length();
        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
        bytemex.append(params.at(1));
    }

    //qDebug()<<'lunghezza array di byte'<<bytemex.size();
    webSocket.sendBinaryMessage( bytemex);
    int i;
    i=0;
}

void socketManager::onConnected()
{
    connect(&webSocket, &QWebSocket::textMessageReceived,
            this, &socketManager::onTextMessageReceived);
    connect(&webSocket, &QWebSocket::binaryMessageReceived,
            this, &socketManager::onBinaryMessageReceived);
    //webSocket.sendTextMessage(QStringLiteral("Hello, world!"));

    QByteArray a("Test start");
    long long n = 0;
    //n = webSocket.sendBinaryMessage(a);

    Message *m = new Message(QChar('I'));
//    Symbol *symbol = new Symbol();
//    symbol->setValue('a');
//    symbol->setSiteId(5);
//    symbol->setCounter(1);
//    std::vector<int> v;
//    v.push_back(2);
//    v.push_back(3);
//    symbol->setPosizione(v);
//    m->setSymbol(symbol);
    Symbol symbol;
    symbol.setValue('a');
    symbol.setSiteId(5);
    symbol.setCounter(1);
    std::vector<int> v;
    v.push_back(2);
    v.push_back(3);
    symbol.setPosizione(v);
    m->setSymbol(symbol);

    //binaryMessageToServer(m);
    //qDebug() << "Numero byte inviati: "<< n;

    qDebug() << "socket Connected";

}

void socketManager::onTextMessageReceived(QString message)
{
Message m=Message::fromJson(QJsonDocument::fromJson(message.toUtf8()));
switch (m.getAction().toLatin1()) {

case 'L':
    if(m.getError()){
        QString s="errore";
        emit(receivedLogin(s));
    }
    else{
        QString s="accesso";
        emit(receivedLogin(s));
        emit(receivedInfoAccount(m));
    }


}
}

//Received binary message from server and emit a signal
void socketManager::onBinaryMessageReceived(QByteArray bytemex)
{
    QByteArray c;
    int tmp, d,sender;
    QChar action;
    QChar tmpc;
    //Symbol *symbol = new Symbol();
    Symbol symbol;
    QVector<QString> params;

    bool emitS = true;

    QString family;
    bool it,bo,un;

    if(bytemex.at(0)=='I'||bytemex.at(0)=='D'){
        action=bytemex.at(0);
        std::vector<int> vtmp;
        int i=2;
        while(bytemex.at(i)!='}'){
            c.clear();
            c.append(bytemex.mid(i,4));
            memcpy(&tmp,c,4);
            vtmp.push_back(tmp);
            i+=4;
        }
        i++;
        symbol.setPosizione(vtmp);
        symbol.setSiteId((int)bytemex.at(i++));
        c.clear();
        c.append(bytemex.mid(i,4));
        memcpy(&tmp,c,4);
        symbol.setCounter(tmp);
        i+=4;
        c.clear();
        c.append(bytemex.mid(i,2));
        memcpy(&tmpc,c,2);
        symbol.setValue(tmpc);
        i+=2;
        bo=bytemex.at(i);
        i++;
        un=bytemex.at(i);
        i++;
        it=bytemex.at(i);
        i++;
        c.clear();
        c.append(bytemex.mid(i,4));
        memcpy(&tmp,c,4);
        d=tmp;
        i+=4;
        c.clear();
        c.append(bytemex.mid(i,4));
        memcpy(&tmp,c,4);
        sender=tmp;
        i+=4;
        family=bytemex.right(bytemex.length()-i);
    }
    else if(bytemex.at(0)=='C'||bytemex.at(0)=='R'){
        if(bytemex.at(0)=='C')
            action='C';
        else
            action='R';
        c.clear();
        c.append(bytemex.mid(1,4));
        memcpy(&tmp,c,4);
        sender=tmp;
        params.push_back(bytemex.right(bytemex.length()-5));
    }
    else if(bytemex.at(0)=='L'){
        action='L';
        c.clear();
        c.append(bytemex.mid(1,4));
        memcpy(&tmp,c,4);
        sender=tmp;
        c.clear();
        c.append(bytemex.mid(5,4));
        memcpy(&tmp,c,4);
        params.push_back(bytemex.mid(9,tmp));
        c.clear();
        c.append(bytemex.mid(tmp+9,4));
        memcpy(&tmp,c,4);
        params.push_back(bytemex.right(tmp));
    }
    else if(bytemex.at(0)=='S'){
        action='S';
        c.clear();
        c.append(bytemex.mid(1,4));
        memcpy(&tmp,c,4);
        sender=tmp;
        c.clear();
        c.append(bytemex.mid(5,4));
        memcpy(&tmp,c,4);
        params.push_back(bytemex.mid(9,tmp));

        emitS = false;
        socketManager::siteId = params.at(0).toInt();

        qDebug() << "siteId received = " <<siteId;
    }

    if (emitS) {
        Message *m = new Message;
        m->setAction(action);
        m->setParams(params);
        m->setSender(sender);
        symbol.setBold(bo);
        symbol.setSize(d);
        symbol.setItalic(it);
        symbol.setUnderln(un);
        symbol.setFamily(family);
        m->setSymbol(symbol);





        emit newMessage(m);
    }

}





