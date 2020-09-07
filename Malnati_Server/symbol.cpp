#include "symbol.h"



Symbol::Symbol(std::vector<int> posizione, int siteId, int counter)
{
    this->posizione = std::move(posizione);
    this->siteId = siteId;
    this->counter = counter;

}

Symbol::Symbol()
{

}
QChar Symbol::getValue() const
{
    return value;
}

void Symbol::setValue(QChar v)
{
    value = v;
}

int Symbol::getSiteId() const
{
    return siteId;
}

void Symbol::setSiteId(int value)
{
    siteId = value;
}

int Symbol::getCounter() const
{
    return counter;
}

void Symbol::setCounter(int value)
{
    counter = value;
}

std::vector<int> Symbol::getPosizione() const
{
    return posizione;
}

void Symbol::setPosizione(const std::vector<int> &value)
{
    posizione = value;
}
