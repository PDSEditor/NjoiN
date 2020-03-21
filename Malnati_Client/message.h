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
    QByteArray bytemex;


public:
    Message(QString action): action(action) {}
    Message();


    QString getAction() const;
    void setAction(const QString &value);

    Symbol *getSymbol() const;
    void setSymbol(Symbol *value);
    QString getParam() const;
    void setParam(const QString &value);
    QByteArray sendmex(char c);
    void recivemex();
};

#endif // MESSAGE_H
