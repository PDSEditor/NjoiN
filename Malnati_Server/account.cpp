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

Account::Account(){

}

Account::Account(QString username, int siteId, QByteArray image): username(username), siteId(siteId), image(image)
{}

Account::Account(const Account &other){
    this->siteId = other.siteId;
    this->username = other.username;
    this->image = other.image;
}

QString Account::toString(){
    return "username: " + this->username + " siteId: " + QString::number(this->siteId);
}
