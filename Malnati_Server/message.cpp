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

QString Message::getSender() const
{
    return sender;
}

void Message::setSender(const QString &value)
{
    sender = value;
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
           <<" position="<<this->getSymbol().getPosition()<<" siteId="<<this->getSymbol().getSiteId()
          <<"counter="<<this->getSymbol().getCounter();
}
