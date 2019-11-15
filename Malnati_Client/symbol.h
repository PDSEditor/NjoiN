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
