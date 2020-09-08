#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <QtCore/QList>

#include "symbol.h"
#include "string.h"
#include <QtCore/QList>
#include <QVector>



class Symbol;

class Message
{

private:
    Symbol symbol;
     QChar action;
     QVector<QString> params;


public:
    Message(QChar action): action(action) {}
    Message();
    ~Message();

    QChar getAction() const;
    void setAction(const QChar &value);
    Symbol getSymbol() const;
    void setSymbol(Symbol value);
    QVector<QString> getParams() const;
    void setParams(const QVector<QString> &value);

};

#endif // MESSAGE_H
