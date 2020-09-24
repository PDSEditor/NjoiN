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
     //informazioni del testo
     QString family;
     bool bold;
     bool italic;
     bool underln;
     //QString size;
     qreal size;


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

    QString getFamily();
    bool getBold();
    bool getItalic();
    bool getUnderln();
    qreal getSize();
    //QString getSize();
    void setFamily(QString f);
    void setBold(bool b);
    void setItalic(bool i);
    void setUnderln(bool u);
    void setSize(qreal s);
    //void setSize(QString s);


};

#endif // MESSAGE_H
