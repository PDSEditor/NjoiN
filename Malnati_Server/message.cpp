#include "message.h"


QChar Message::getAction() const
{
    return action;
}

void Message::setAction(const QChar &value)
{
    action = value;
}


Symbol *Message::getSymbol() const
{
    return symbol;
}

void Message::setSymbol(Symbol *value)
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

Message::Message()
{
    symbol =new Symbol();
}
