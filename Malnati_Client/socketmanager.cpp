#include "socketmanager.h"

socketManager::socketManager(const QUrl &url,  QObject *parent) : QObject(parent)
{
    //url = *(new QUrl("localhost:1234"));
    connect(&webSocket, &QWebSocket::connected, this, &socketManager::onConnected);
    //connect(webSocket, &QWebSocket::disconnected, this, &socketManager::closed);
    //webSocket= new QWebSocket();
    webSocket.open(QUrl(url));
    //qDebug()<<webSocket.isValid();
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

void socketManager::binaryMessageToServer(Message *m)
{
    //qDebug()<<"Testo ricevuto: ";

    int tmp;
    QByteArray bytemex;
    QChar action = m->getAction();
    Symbol *symbol = m->getSymbol();



    if(action==('I')||action==("D")){
        if(action==('I')){
            bytemex.append('I');
        }
        else{
            bytemex.append('D');
        }
        bytemex.append('[');
        for(unsigned long long i=0;i<symbol->getPosizione().size();i++){
            tmp=(symbol->getPosizione().at(i));
            for(int p=0;p<4;p++){
                bytemex.append(tmp >> (p * 8));
            }
        }
        bytemex.append(']');
        bytemex.append(symbol->getSiteId());//dimensione massima
        tmp=(symbol->getCounter());
        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
        bytemex.append(symbol->getValue());

    }
    else if(action==('C')||action==('R')){
        QVector<QString> params = m->getParams();
        if(action==('C')){
            bytemex.append('C');
        }
        else{
            bytemex.append('R');
        }
        bytemex.append(params.at(0));

    }
    else if(action=='L'){
        QVector<QString> params = m->getParams();
        bytemex.append('L');
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
    //webSocket.sendTextMessage(QStringLiteral("Hello, world!"));

    QByteArray a("Test start");
    long long n = 0;
   // n = webSocket.sendBinaryMessage(a);

    Message *m = new Message(QChar('I'));
    Symbol *symbol = new Symbol();
    symbol->setValue('a');
    symbol->setSiteId(5);
    symbol->setCounter(1);
    std::vector<int> v;
    v.push_back(2);
    v.push_back(3);
    symbol->setPosizione(v);
    m->setSymbol(symbol);

    //binaryMessageToServer(m);
    //qDebug() << "Numero byte inviati: "<< n;

    qDebug() << "socket Connected";

}

void socketManager::onTextMessageReceived(QString message)
{

}

void socketManager::onBinaryMessageReceived(QByteArray bytemex)
{
    QByteArray c;
    int tmp;
    QChar action;
    Symbol *symbol = new Symbol();
    QVector<QString> params;

    if(bytemex.at(0)=='I'||bytemex.at(0)=='D'){
        std::vector<int> vtmp;
        int i=2;
        while(bytemex.at(i)!=']'){
            c.clear();
            c.append(bytemex.mid(i,4));
            memcpy(&tmp,c,4);
            vtmp.push_back(tmp);
            i+=4;
        }
        i++;
        symbol->setPosizione(vtmp);
        symbol->setSiteId((int)bytemex.at(i++));
        c.clear();
        c.append(bytemex.mid(i,4));
        memcpy(&tmp,c,4);
        symbol->setCounter(tmp);
        i+=4;
        symbol->setValue(bytemex.at(i));
    }
    else if(bytemex.at(0)=='C'||bytemex.at(0)=='R'){
        if(bytemex.at(0)=='C')
            action='C';
        else
            action='R';
        params.push_back(bytemex.right(bytemex.length()-1));
    }
    else if(bytemex.at(0)=='L'){
        action='L';
        c.clear();
        c.append(bytemex.mid(1,4));
        memcpy(&tmp,c,4);
        params.push_back(bytemex.mid(5,tmp));
        c.clear();
        c.append(bytemex.mid(tmp+5,4));
        memcpy(&tmp,c,4);
        params.push_back(bytemex.right(tmp));
    }

    Message *m = new Message;
    m->setAction(action);
    m->setParams(params);
    m->setSymbol(symbol);

    emit newMessage(m);


}


