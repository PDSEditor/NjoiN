#ifndef SYMBOL_H
#define SYMBOL_H

#include <vector>
#include <QChar>


class Symbol
{

private:
    QChar value;
    std::vector<int> posizione;
    int siteId;
    int counter;

public:
    Symbol(std::vector<int> posizione, int siteId, int counter);
    Symbol();

    std::vector<int> getPosizione() const;
    void setPosizione(const std::vector<int> &value);
    int getSiteId() const;
    void setSiteId(int value);
    int getCounter() const;
    void setCounter(int value);
    QChar getValue() const;
    void setValue(QChar v);
};

#endif // SYMBOL_H
