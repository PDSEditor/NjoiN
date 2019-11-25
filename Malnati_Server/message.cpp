#include "message.h"


QString Message::getAction() const
{
    return action;
}

void Message::setAction(const QString &value)
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

QString Message::getParam() const
{
    return param;
}

void Message::setParam(const QString &value)
{
    param = value;
}

Message::Message()
{
    symbol =new Symbol();
}

