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
    Symbol *symbol;
     QChar action;
     QVector<QString> params;

    QByteArray bytemex;


public:
    Message(QChar action): action(action) {}
        Message();


        QChar getAction() const;
        void setAction(const QChar &value);

        Symbol *getSymbol() const;
        void setSymbol(Symbol *value);
        QVector<QString> getParams() const;
        void setParams(const QVector<QString> &value);
        QByteArray getArray() const;
        void setArray(QByteArray array);
        QByteArray sendmex();
        void recivemex();
};

#endif // MESSAGE_H
