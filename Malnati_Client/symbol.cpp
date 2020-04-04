#include "symbol.h"

Symbol::Symbol(char value, std::vector<int> posizione, int siteId, int counter): value(value), siteId(siteId), counter(counter)
{
    //this->value = value;
    this->posizione = std::move(posizione); /*da vedere, in realtà si chiama la std::move direttamente dal cpp e
                                              poi in automatico viene invocato il costruttore di movimento*/
    //this->siteId = siteId;
    //this->counter = counter;
}
Symbol::Symbol(){

}
char Symbol::getValue() const{
    return value;
}
void Symbol::setValue(char value){
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
