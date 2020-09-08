#include "message.h"


QChar Message::getAction() const
{
    return action;
}

void Message::setAction(const QChar value)
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

void Message::setParams(const QVector<QString> value)
{
    params = value;
}

Message::Message()
{
    Symbol symbol;
    this->symbol=std::move(symbol);
}

void Message::debugPrint()
{
    //Aggiungere parametri
    qDebug()<<"Message->Action: "<<this->getAction()<<" Symbol: value="<<this->getSymbol().getValue()
           <<" position="<<this->getSymbol().getPosizione()<<" siteId="<<this->getSymbol().getSiteId()
          <<"counter="<<this->getSymbol().getCounter();
}
