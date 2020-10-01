#include "documentManager.h"

DocumentManager::DocumentManager(QObject *parent) : QObject(parent)
{

}

bool DocumentManager::saveToServer(QString documentId)
{
//    QFile file(document.getUri()); //uri? o name? o id?
//    if (!file.open(QFile::WriteOnly)) return false;
//    QByteArray bArray;
//    for(auto i : document.getSymbols()){
//        bArray.append(i.getValue());
//    }

//    auto returnValue = file.write(bArray);
//    if(returnValue == -1)
        return false;
    return true;
}

bool DocumentManager::checkPermission(QString username, QString documentId)
{
    QList<SharedDocument>::iterator it;
    for(it = this->openDocuments.begin(); it!=this->openDocuments.end(); it++) {
        QString name = it->getName();
        QString creator = it->getCreator();
        if((name+'_'+creator) == documentId) {
            auto userAllowed = it->getUserAllowed();

            if(userAllowed.contains(username)){
                return true;
            }
        }
    }
    return false;
}
