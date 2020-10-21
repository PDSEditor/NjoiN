#ifndef SYMBOL_H
#define SYMBOL_H

#include <vector>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <iostream>


class Symbol
{

private:
    QChar value;
    std::vector<int> position;
    int siteId;
    int counter;

    QString family;
    bool bold;
    bool italic;
    bool underln;
    qreal size;
    QChar align;


public:
    Symbol(std::vector<int> &position, int siteId, int counter);
    Symbol(QChar value, std::vector<int> &position, int siteId, int counter);
    Symbol();

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

    QChar getAlign();
    void setAlign(QChar);
    std::vector<int> getPosition() const;
    void setPosition(const std::vector<int> &value);
    int getSiteId() const;
    void setSiteId(int value);
    int getCounter() const;
    void setCounter(int value);
    QChar getValue() const;
    void setValue(QChar v);
    static Symbol fromJson(const QJsonDocument &charJD);
    QJsonDocument toJson();
    bool operator < (const Symbol &other) const;

};

#endif // SYMBOL_H

