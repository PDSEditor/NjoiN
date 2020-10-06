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

void AccountManager::removeOnlineAccounts(int siteId)
{
        if(this->onlineAccounts.contains(siteId)) {
            this->onlineAccounts.remove(siteId);
        }
        else {
            qDebug("rimozione di un account con siteId ") ;
        }
}
