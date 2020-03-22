#include "message.h"
#include<cstring>



QChar Message::getAction() const
{
    return action;
}

void Message::setAction(const QChar &value)
{
    action = value;
}


Symbol *Message::getSymbol() const
{
    return symbol;
}

void Message::setSymbol(Symbol *value)
{
    symbol = value;
}

QVector<QString> Message::getParams() const
{
    return params;
}

void Message::setParams(const QVector<QString> &value)
{
    params = value;
}

Message::Message()
{
    symbol =new Symbol();
}

QByteArray Message::getArray() const
{
    return bytemex;

}
void Message::setArray(QByteArray array)
{
    bytemex=array;
}

QByteArray Message::sendmex(){
    int tmp;
    if(action==("i")||action==("d")){
        if(action==("i")){
            bytemex.append('i');
        }
      else{
            bytemex.append('d');
        }
      bytemex.append('[');
      for(unsigned long long i=0;i<symbol->getPosizione().size();i++){
          tmp=(symbol->getPosizione().at(i));
          for(int p=0;p<4;p++){
              bytemex.append(tmp >> (p * 8));
          }
      }
          bytemex.append(']');
          bytemex.append(symbol->getSiteId());//dimensione massima
          tmp=(symbol->getCounter());
          for(int p=0;p<4;p++){
              bytemex.append(tmp >> (p * 8));
          }

    }
    else if(action==('c')||action==('r')){
        if(action==('c')){
            bytemex.append('c');
        }
        else{
            bytemex.append('d');
        }
        bytemex.append(params.at(0));

    }
    else if(action=='l'){
        bytemex.append('l');
        tmp=params.at(0).length();
        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
        bytemex.append(params.at(0));
        tmp=params.at(1).length();
        for(int p=0;p<4;p++){
            bytemex.append(tmp >> (p * 8));
        }
        bytemex.append(params.at(1));
    }


    return bytemex;
}

void Message::recivemex(){
        QByteArray c;
        int tmp;
    if(bytemex.at(0)=='i'||bytemex.at(0)=='d'){
        std::vector<int> vtmp;
        int i=2;
        while(bytemex.at(i)!=']'){
            c.clear();
            c.append(bytemex.mid(i,4));
            std::memcpy(&tmp,c,4);
            vtmp.push_back(tmp);
            i+=4;
        }
        i++;
        symbol->setPosizione(vtmp);
        symbol->setSiteId((int)bytemex.at(i++));
        c.clear();
        c.append(bytemex.mid(i,4));
        std::memcpy(&tmp,c,4);
        symbol->setCounter(tmp);
    }
    else if(bytemex.at(0)=='c'||bytemex.at(0)=='r'){
        if(bytemex.at(0)=='c')
            action='c';
        else
            action='r';
        params.push_back(bytemex.right(bytemex.length()-1));
    }
    else if(bytemex.at(0)=='l'){
        action='l';
        c.clear();
        c.append(bytemex.mid(1,4));
        std::memcpy(&tmp,c,4);
        params.push_back(bytemex.mid(5,tmp));
        c.clear();
        c.append(bytemex.mid(tmp+5,4));
        std::memcpy(&tmp,c,4);
        params.push_back(bytemex.right(tmp));
    }

}
