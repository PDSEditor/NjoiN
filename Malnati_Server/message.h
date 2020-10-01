#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <QtCore/QList>
#include <QVector>
#include <QDebug>

#include "symbol.h"


class Symbol;

class Message
{

private:
    int sender;
    Symbol symbol;
    QChar action;
    QVector<QString> params;





public:
    Message(QString sender, QChar action);
    Message(QChar action): action(action) {}
    Message();

    void debugPrint();


    QChar getAction() const;
    void setAction(const QChar &value);

    Symbol getSymbol() const;
    void setSymbol(Symbol value);
    QVector<QString> getParams() const;
    void setParams(const QVector<QString> &value);


    int getSender() const;
    void setSender(const int &value);

};

#endif // MESSAGE_H
