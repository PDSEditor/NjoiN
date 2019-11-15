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

