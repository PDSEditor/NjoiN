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
    bool error;

public:
    Message(int sender, QChar action): sender(sender), action(action) {};
    Message(QChar action): action(action) {};
    Message();

    void debugPrint();
    static Message fromJson(QJsonDocument &json);
    QJsonDocument toJson();

    QChar getAction() const;
    void setAction(const QChar &value);

    Symbol getSymbol() const;
    void setSymbol(Symbol &value);
    QVector<QString> getParams() const;
    void setParams(const QVector<QString> &value);
    void addParam(QString s);

    int getSender() const;
    void setSender(int value);
    bool getError() const;
    void setError(bool value);
};

#endif // MESSAGE_H
