#ifndef MESSAGE_H
#define MESSAGE_H

#include <QtCore/QString>
#include <QtCore/QList>
#include "symbol.h"


class Message
{

private:
    QString action;
    QList<Symbol> symbols;

public:
    Message();

};

#endif // MESSAGE_H
