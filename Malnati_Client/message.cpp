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
}

Message::~Message(){
    //delete symbol;
}



