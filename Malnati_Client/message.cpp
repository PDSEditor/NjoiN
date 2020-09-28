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


Message::Message()
{
    Symbol symbol;
    this->symbol=std::move(symbol);
}

Message::~Message(){
    //delete symbol;
}



