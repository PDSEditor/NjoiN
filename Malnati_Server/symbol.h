<<<<<<< HEAD
#ifndef SYMBOL_H
#define SYMBOL_H

#include <vector>


class Symbol
{

private:
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
};

#endif // SYMBOL_H
=======
#ifndef SYMBOL_H
#define SYMBOL_H

#include <vector>


class Symbol
{

private:
    std::vector<int> posizione;
    int siteId;
    int counter;

public:
    Symbol(std::vector<int> posizione, int siteId, int counter);

    std::vector<int> getPosizione() const;
    void setPosizione(const std::vector<int> &value);
    int getSiteId() const;
    void setSiteId(int value);
    int getCounter() const;
    void setCounter(int value);
};

#endif // SYMBOL_H
>>>>>>> 3e4cc998bb356e7d0d56f393ca13f3a6e9896e64
