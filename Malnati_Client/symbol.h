#ifndef SYMBOL_H
#define SYMBOL_H
#include <vector>

class Symbol
{
private:
    char value;
    std::vector<int> posizione;
    int siteId;
    int counter;
public:
    Symbol(char value, std::vector<int> posizione, int siteId, int counter);
    Symbol();
        char getValue() const;
        void setValue(char value);
        std::vector<int> getPosizione() const;
        void setPosizione(const std::vector<int> &value);
        int getSiteId() const;
        void setSiteId(int value);
        int getCounter() const;
        void setCounter(int value);
        bool operator!=(const Symbol &symbol){
            if(this->siteId!=symbol.siteId || this->value!=symbol.value || this->counter!=symbol.counter)
                return true;
            else return false;
        }
        bool operator==(const Symbol &symbol){
            if(this->siteId==symbol.siteId && this->value==symbol.value && this->counter==symbol.counter)
                return true;
            else return false;
        }
};

#endif // SYMBOL_H
