#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <QtCore/QList>

#include "symbol.h"
#include "string.h"
#include <QtCore/QList>
#include <QVector>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>



class Symbol;

class Message
{

private:
    int sender;
    Symbol symbol;
     QChar action;
     QVector<QString> params;
     bool error;

public:
    Message(QChar action): action(action) {}
    Message();
    ~Message();

    QChar getAction() const;
    void setAction(const QChar &value);
    static Message fromJson(QJsonDocument json);
    QJsonDocument toJson();

    Symbol getSymbol() const;
    void setSymbol(Symbol value);
    QVector<QString> getParams() const;
    void setParams(const QVector<QString> &value);

    QString getFamily();
    bool getBold();
    bool getItalic();
    bool getUnderln();
    qreal getSize();
    int getSender() const;
    void setSender(const int &value);
    void setError(bool e);
    bool getError();

    void setFamily(QString f);
    void setBold(bool b);
    void setItalic(bool i);
    void setUnderln(bool u);
    void setSize(qreal s);



};

#endif // MESSAGE_H

