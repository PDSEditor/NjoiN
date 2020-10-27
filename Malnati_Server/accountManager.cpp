#include "accountManager.h"


QMap<int, std::shared_ptr<Account> > AccountManager::getOnlineAccounts() const
{
    return onlineAccounts;
}

void AccountManager::setOnlineAccounts(const QMap<int, std::shared_ptr<Account> > &value)
{
    onlineAccounts = value;
}

QMap<QString, QList<QString> > AccountManager::getAccountsPerFile() const
{
    return accountsPerFile;
}

void AccountManager::setAccountsPerFile(const QMap<QString, QList<QString> > &value)
{
    accountsPerFile = value;
}


QMap<QString, QString> AccountManager::getUsernameDocumentMap() const
{
    return usernameDocumentMap;
}

void AccountManager::setUsernameDocumentMap(const QMap<QString, QString> &value)
{
    usernameDocumentMap = value;
}

bool AccountManager::removeUsernameFromMap(QString username)
{
    if(this->usernameDocumentMap.remove(username))
            return true;
    else return false;
}

AccountManager::AccountManager(QObject *parent) : QObject(parent)
{
    
    
}


bool AccountManager::closeDocumentByUser(QString username, QString documentId)
{
    if(this->accountsPerFile.contains(documentId)){
        QList<QString>* it = &this->accountsPerFile[documentId];
        if(it->contains(username)){
            it->removeOne(username);

            if(it->count()==0) {
                //bisogna salvare il documento
                return false;
            }
        }
    }
    return true;

}

bool AccountManager::updateOnlineAccounts(int siteId, const Account& acc)
{

    if(!this->onlineAccounts.contains(siteId)) {     //account che si collega per la prima volta
        //implementare l'inserimento anche nel db
        auto acc_p = std::make_shared<Account>(acc);
        this->onlineAccounts.insert(siteId,acc_p);
        return true;
    }
    else{
        return false;
    }

}

void AccountManager::updateAccountOnDocument(QString user, QString documentId)
{
    this->usernameDocumentMap[user] = documentId;
}

void AccountManager::removeOnlineAccounts(int siteId)
{
        QString username;

        if(this->onlineAccounts.contains(siteId)) {
            username = this->onlineAccounts.value(siteId).get()->getUsername();
            this->onlineAccounts.remove(siteId);

// rimuovo anche l'utente dalla lista che tiene conto degli utenti online sui file attualmente aperti

            for(auto iter = this->accountsPerFile.begin(); iter != accountsPerFile.end(); ++iter) {

                if(iter.value().contains(username)){
                    iter.value().removeOne(username);
                }

            }
        }
        else {
            qDebug("rimozione di un account con siteId non presente tra quelli accesi") ;
        }


}
