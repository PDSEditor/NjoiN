#include "message.h"


QChar Message::getAction() const
{
    return action;
}

void Message::setAction(const QChar &value)
{
    action = value;
}


Symbol Message::getSymbol() const
{
    return symbol;
}

void Message::setSymbol(Symbol &value)
{
    symbol = value;
}

QVector<QString> Message::getParams() const
{
    return params;
}

void Message::setParams(const QVector<QString> &value)
{
    params = value;
}


QString Message::getFamily()
{
    return family;
}

bool Message::getBold()
{
    return bold;
}

bool Message::getItalic()
{
    return italic;
}

bool Message::getUnderln()
{
    return underln;
}

qreal Message::getSize()
{
    return size;
}

void Message::setFamily(QString f)
{
    family=f;
}

void Message::setBold(bool b)
{
    bold=b;
}

void Message::setItalic(bool i)
{
    italic=i;
}

void Message::setUnderln(bool u)
{
    underln=u;
}

void Message::setSize(qreal s)
{
    size=s;
}

int Message::getSender() const
{
    return sender;
}

void Message::setSender(int value)
{
    sender = value;
}

bool Message::getError() const
{
    return error;
}

void Message::setError(bool value)
{
    error = value;
}

Message::Message()
{
    Symbol symbol;
    this->symbol=std::move(symbol);
    this->error = false;
}

void Message::debugPrint()
{
    //Aggiungere parametri
    qDebug()<<"Message->Action: "<<this->getAction()<<" Symbol: value="<<this->getSymbol().getValue()
           <<" position="<<this->getSymbol().getPosition()<<" siteId="<<this->getSymbol().getSiteId()
          <<"counter="<<this->getSymbol().getCounter();
}

Message Message::fromJson(QJsonDocument &json)
{
    QJsonObject json_obj = json.object();
    Message m = Message();
    m.setSender(json_obj["sender"].toInt());
    m.setAction(json_obj["action"].toInt());
    m.setError(json_obj["error"].toBool());
    QVector<QString> vec;

    if(json_obj.contains("params") && json_obj["params"].isArray()) {
        QJsonArray array = json_obj["params"].toArray();
        for(int i=0; i<array.size(); i++){
            vec.append(array[i].toString());
        }
    }
    m.setParams(vec);
    return m;

}

QJsonDocument Message::toJson()
{
    QJsonObject json_obj;
    json_obj["sender"] = this->sender;
    //json_obj["symbol"] = this->symbol;    se stiamo inviando il symbol siamo nel caso binario e quindi non usiamo json
    json_obj["action"] = this->action.toLatin1();
    json_obj["error"] = this->error;

    QJsonArray paramsj;
    for(QString param : this->params) {
        paramsj.append(param);
    }
    json_obj["params"] = paramsj;
    return QJsonDocument(json_obj);
}


