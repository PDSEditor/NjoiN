#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include "symbol.h"


class Symbol;

class Message
{

private:
    Symbol* simbolo;
    std::string action;


public:
    Message(Symbol* simbolo, std::string action): simbolo(simbolo), action(action) {}
    Message();


    std::string getAction() const;
    void setAction(const std::string &value);
    Symbol *getSimbolo() const;
    void setSimbolo(Symbol *value);
};

#endif // MESSAGE_H
