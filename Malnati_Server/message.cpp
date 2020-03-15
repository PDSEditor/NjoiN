<<<<<<< HEAD
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

=======
#include "message.h"


Symbol *Message::getSimbolo() const
{
    return simbolo;
}

void Message::setSimbolo(Symbol *value)
{
    simbolo = value;
}

Message::Message()
{

}

std::string Message::getAction() const
{
    return action;
}

void Message::setAction(const std::string &value)
{
    action = value;
}

>>>>>>> 3e4cc998bb356e7d0d56f393ca13f3a6e9896e64
