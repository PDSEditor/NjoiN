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

    QString family;
    bool bold;
    bool italic;
    bool underln;
    qreal size;



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

    QString getFamily();
    bool getBold();
    bool getItalic();
    bool getUnderln();
    qreal getSize();
    void setFamily(QString f);
    void setBold(bool b);
    void setItalic(bool i);
    void setUnderln(bool u);
    void setSize(qreal s);


    int getSender() const;
    void setSender(const int &value);
};

#endif // MESSAGE_H
