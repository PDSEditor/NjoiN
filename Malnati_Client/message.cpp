#include "message.h"
#include<cstring>



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

void Message::setSymbol(Symbol value)
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
int Message::getSender() const
{
    return sender;
}

void Message::setSender(const int &value)
{
    sender = value;
}

Message::Message()
{
    Symbol symbol;
    this->symbol=std::move(symbol);
    this->error=false;
}

Message::~Message(){
    //delete symbol;
}

void Message::setError(bool e){
    error=e;
}
bool Message::getError(){
    return error;
}

Message Message::fromJson(QJsonDocument json)
{
    QJsonObject json_obj = json.object();
    Message m = Message();
    m.setSender(json_obj["sender"].toInt());
    m.setAction(json_obj["action"].toInt());
    QVector<QString> vec;

    if(json_obj.contains("params") && json_obj["params"].isArray()) {
        QJsonArray array = json_obj["params"].toArray();
        for(int i=0; i<array.size(); i++){
            vec.append(array[i].toString());
        }
    }
    m.setParams(vec);
    m.setError(json_obj["error"].toBool());
    return m;

}

QJsonDocument Message::toJson()
{
    QJsonObject json_obj;
    json_obj["sender"] = this->sender;
    //json_obj["symbol"] = this->symbol;    se stiamo inviando il symbol siamo nel caso binario e quindi non usiamo json
    json_obj["action"] = this->action.toLatin1();

    QJsonArray paramsj;
    for(QString param : this->params) {
        paramsj.append(param);
    }
    json_obj["params"] = paramsj;
    json_obj["error"] = this->error;
    return QJsonDocument(json_obj);
}



