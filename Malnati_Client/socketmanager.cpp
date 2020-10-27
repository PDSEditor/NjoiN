#include "socketmanager.h"

socketManager::socketManager(const QUrl &url,  QObject *parent) : QObject(parent)
{
    connect(&webSocket, &QWebSocket::connected, this, &socketManager::onConnected); 
    webSocket.open(QUrl(url));
    if(webSocket.isValid())
        this->setServerOn(true);
    else this->setServerOn(false);
}

socketManager::~socketManager()
{
    this->webSocket.close();
}

void socketManager::messageToServer(Message *m)
{
    if(!this->getServerOn()){
        qDebug() << "Server is down!";
        emit(loggedin(false));
    }

    webSocket.sendTextMessage(m->toJson().toJson(QJsonDocument::Compact));
}

void socketManager::closingSock(char* error)
{
    qDebug()<<"chiudendo socket";
}

bool socketManager::getServerOn() const
{
    return serverOn;
}

void socketManager::setServerOn(bool value)
{
    serverOn = value;
}


//Send a message from client to server
void socketManager::binaryMessageToServer(Message *m)
{
    int tmp;
    QChar tmpc;
    QByteArray bytemex;
    QChar action = m->getAction();
    QVector<QString> params;
    m->setSender(this->siteId);

    Symbol symbol = m->getSymbol();


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
        bytemex.append(m->getSender());//dimensione massima
        tmp=(symbol.getCounter());

        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
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
        tmp=m->getSender();
        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
        bytemex.append(symbol.getAlign());
        bytemex.append(symbol.getFamily());


    }
    webSocket.sendBinaryMessage( bytemex);
}

void socketManager::onConnected()
{
    connect(&webSocket, &QWebSocket::textMessageReceived,
            this, &socketManager::onTextMessageReceived);
    connect(&webSocket, &QWebSocket::binaryMessageReceived,
            this, &socketManager::onBinaryMessageReceived);

    QByteArray a("Test start");
    Message *m = new Message(QChar('I'));
    Symbol symbol;
    symbol.setValue('a');
    symbol.setSiteId(5);
    symbol.setCounter(1);
    std::vector<int> v;
    v.push_back(2);
    v.push_back(3);
    symbol.setPosizione(v);
    m->setSymbol(symbol);
    serverOn=true;
    qDebug() << "socket Connected";

}

void socketManager::onTextMessageReceived(QString message)
{
    Message m=Message::fromJson(QJsonDocument::fromJson(message.toUtf8()));
    QList<Symbol> listtmp;

    switch (m.getAction().toLatin1()) {
    case 'L':
        if(m.getError()){
            if( m.getParams().length()!=0 && m.getParams().at(0)=="2" )
                emit(loggedin(true));
            else
                emit(receivedLogin(false));
        }
        else{
            this->siteId = m.getSender();
            emit(receivedLogin(true));
            emit(receivedInfoAccount(m));
        }
        break;
    case 'S': //per settare il siteId

        emit(setSiteId(m.getSender()));
        break;
    case 'E'://Registrazione
        emit(receiveRegistration(m));
        break;
    case 'R':

        foreach (QString s, m.getParams()) {
            auto stmp=Symbol::fromJson(QJsonDocument::fromJson(s.toUtf8()));
            listtmp.append(stmp);
        }
        emit(receivedFile(listtmp));
        break;
    case 'U':
        if(!m.getError()){
            foreach (QString s, m.getParams()) {
                auto stmp=Symbol::fromJson(QJsonDocument::fromJson(s.toUtf8()));
                listtmp.append(stmp);
            }
            emit(receivedFile(listtmp));
        }
        else{
            emit(receivedURIerror());
        }
        break;

    case 'G':{
        // ricevo risultato del cambio immagine
        emit(receiveNewImage(m));
        break;
    }

    case 'A':{
        //aggiorna gli utenti online e offline sul documento aperto dal client
        emit(showUsers(m));
        break;
    }
    case 'P':{
        //ricevo risultato del cambio password
        emit(receiveNewPsw(m));
        break;
    }
    case 'B':{
        emit(receiveAllign(m));
        break;
    }
    case 'Z':{
        int pos = m.getParams()[0].toInt();
        QString userId =m.getParams()[1];
        emit(updateCursor(pos,userId));
        break;
    }
    default:{
        qDebug() << "default";
        break;
    }
    }
}

//Received binary message from server and emit a signal
void socketManager::onBinaryMessageReceived(QByteArray bytemex)
{
    QByteArray c;
    int tmp, d,sender;
    QChar action,align;
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
        align=bytemex.at(i);
        i++;
        family=bytemex.right(bytemex.length()-i);
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
        symbol.setAlign(align);
        m->setSymbol(symbol);
        emit newMessage(m);
    }

}





