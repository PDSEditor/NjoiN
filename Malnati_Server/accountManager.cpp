#include "accountManager.h"

QMap<int, Account *> AccountManager::getOnlineAccounts() const
{
    return onlineAccounts;
}

void AccountManager::setOnlineAccounts(const QMap<int, Account *> &value)
{
    onlineAccounts = value;
}

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

void AccountManager::checkUserPerFile(int siteId, QString fileName)
{
    if(!this->accountsPerFile[fileName].contains(siteId)) {     //primo accesso a un file creato da altri
        this->accountsPerFile[fileName].append(siteId);
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
