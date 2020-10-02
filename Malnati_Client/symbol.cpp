#include "symbol.h"

Symbol::Symbol(QChar value, std::vector<int> posizione, int siteId, int counter): value(value), siteId(siteId), counter(counter)
{
    //this->value = value;
    this->posizione = std::move(posizione);
    //this->siteId = siteId;
    //this->counter = counter;
}
Symbol::Symbol(){

}
QChar Symbol::getValue() const{
    return value;
}
void Symbol::setValue(QChar value){
    this->value = value;
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

QString Symbol::getFamily()
{
    return family;
}

bool Symbol::getBold()
{
    return bold;
}

bool Symbol::getItalic()
{
    return italic;
}

bool Symbol::getUnderln()
{
    return underln;
}

qreal Symbol::getSize()
{
    return size;
}

void Symbol::setFamily(QString f)
{
    family=f;
}

void Symbol::setBold(bool b)
{
    bold=b;
}

void Symbol::setItalic(bool i)
{
    italic=i;
}

void Symbol::setUnderln(bool u)
{
    underln=u;
}

void Symbol::setSize(qreal s)
{
    size=s;
}
