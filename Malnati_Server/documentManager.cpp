#include "documentManager.h"

DocumentManager::DocumentManager(QObject *parent) : QObject(parent)
{

}

bool DocumentManager::saveToServer(QString documentId, QList<Symbol> &symbols)
{
    //todo: da testare
    for(auto it : openDocuments){
        if(QString::compare(it.getUri(), documentId) != 0) continue;
        //        QString name = it.getName();
        //        QString creator = it.getCreator();
        //        if((name+'_'+creator) == documentId) {
//        QFile file("/home/pepos/projects/progetto_malnati/Malnati_Server/backup/" + documentId+ ".bin"); //uri? o name? o id?
        QFile file(documentId + ".bin"); //uri? o name? o id?
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
        QByteArray bArray;
        for(auto i : symbols){
            bArray.append(i.toJson().toBinaryData());
            //            }
        }
        auto returnValue = file.write(bArray, bArray.size()); //salva nella cartella di build

        if(returnValue == -1)
            return false;
        else {
            file.close();
//            qDebug() << file.fileName();
            return true;
        }
            //da sistemare le parentesi poi dovrebbe andare
//        }
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
    for (QList<SharedDocument>::iterator doc=openDocuments.begin(); doc != openDocuments.end(); ++doc){
        auto uri = doc->getUri();
        if(uri != nullptr){
            openDocuments.erase(doc);
        }
    }
}
