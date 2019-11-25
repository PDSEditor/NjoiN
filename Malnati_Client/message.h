#ifndef MESSAGE_H
#define MESSAGE_H

#include <QtCore/QString>
#include <QtCore/QList>
#include "symbol.h"


class Message
{

private:
    QString action;
    Symbol *symbol;
    QString param;

public:
    Message();
    Message(QString action);

    QString getAction() const;
    void setAction(const QString &value);
    QString getParam() const;
    void setParam(const QString &value);
    Symbol *getSymbol() const;
    void setSymbol(Symbol *value);
};

#endif // MESSAGE_H
