#include "account.h"

QString Account::getUsername() const
{
    return username;
}

void Account::setUsername(const QString &value)
{
    username = value;
}

int Account::getSiteId() const
{
    return siteId;
}

void Account::setSiteId(int value)
{
    siteId = value;
}

Account::Account()
{
    
}

Account::Account(QString username, int siteId){
    this->username = username;
    this->siteId = siteId;
}

QString Account::toString(){
    return this->username + this->siteId;
}
