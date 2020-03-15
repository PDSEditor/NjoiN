<<<<<<< HEAD
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
=======
#include "symbol.h"

Symbol::Symbol(std::vector<int> posizione, int siteId, int counter)
{
    this->posizione = std::move(posizione);
    this->siteId = siteId;
    this->counter = counter;

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
>>>>>>> 3e4cc998bb356e7d0d56f393ca13f3a6e9896e64
