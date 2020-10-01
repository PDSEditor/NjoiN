#include "documentManager.h"

DocumentManager::DocumentManager(QObject *parent) : QObject(parent)
{

}

bool DocumentManager::saveToServer(SharedDocument &document)
{
    QFile file(document.getUri()); //uri? o name? o id?
    if (!file.open(QFile::WriteOnly)) return false;
    QByteArray bArray;
    for(auto i : document.getSymbols()){
        bArray.append(i.getValue());
    }

    auto returnValue = file.write(bArray);
    if(returnValue == -1)
        return false;
    else return true;
}
