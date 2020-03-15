<<<<<<< HEAD
#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <QtCore/QList>

#include "symbol.h"


class Symbol;

class Message
{

private:
    Symbol *symbol;
    QString action;
    QString param;


public:
    Message(QString action): action(action) {}
    Message();


    QString getAction() const;
    void setAction(const QString &value);

    Symbol *getSymbol() const;
    void setSymbol(Symbol *value);
    QString getParam() const;
    void setParam(const QString &value);
};

#endif // MESSAGE_H
=======
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
>>>>>>> 3e4cc998bb356e7d0d56f393ca13f3a6e9896e64
