#include "documentManager.h"

DocumentManager::DocumentManager(QObject *parent) : QObject(parent)
{

}

bool DocumentManager::saveToServer(QString documentId)
{
    for(auto it : openDocuments){
        QString name = it.getName();
        QString creator = it.getCreator();
        if((name+'_'+creator) == documentId) {
            QFile file(documentId+".bin"); //uri? o name? o id?
            if (!file.open(QFile::WriteOnly)) return false;
            QByteArray bArray;
            for(auto i : it.getSymbols()){
                bArray.append(i.getValue());
            }
            auto returnValue = file.write(bArray);

            if(returnValue == -1)
                return false;
            else return true;
        }
    }
    return false;
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

void DocumentManager::openDocument(SharedDocument &doc)
{
    this->openDocuments.push_back(doc);
}

void DocumentManager::closeDocument(QString uri)
{
    int i=0;
    for(auto doc : openDocuments) {
        if(doc.getUri() == uri)
            openDocuments.removeAt(i);
        i++;
    }
}
