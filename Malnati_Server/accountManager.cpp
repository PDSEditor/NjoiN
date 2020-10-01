#include "accountManager.h"


QMap<int, std::shared_ptr<Account> > AccountManager::getOnlineAccounts() const
{
    return onlineAccounts;
}

void AccountManager::setOnlineAccounts(const QMap<int, std::shared_ptr<Account> > &value)
{
    onlineAccounts = value;
}

AccountManager::AccountManager(QObject *parent) : QObject(parent)
{
    
    
}


bool AccountManager::closeDocumentByUser(QString username, QString documentId)
{
    if(this->accountsPerFile.contains(documentId)){
        auto list = this->accountsPerFile[documentId];
        if(list.contains(username)){
            list.removeOne(username);

            if(list.count()==0) {
                //bisogna salvare il documento
            }
        }
    }

}

void AccountManager::updateOnlineAccounts(int siteId)
{
    Account acc;
    if(!this->accounts.contains(siteId)) {     //account che si collega per la prima volta
        //implementare l'inserimento anche nel db
        this->accounts.insert(siteId, acc);

    }

//    acc = &accounts[siteId];
//    this->onlineAccounts.insert(siteId, acc);
}

void AccountManager::removeOnlineAccounts(int siteId)
{

}
