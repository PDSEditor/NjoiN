#include "account.h"

QList<QString> Account::getDocumentUris() const
{
    return documentUris;
}

void Account::setDocumentUris(const QList<QString> &value)
{
    documentUris = value;
}

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

Account::Account(QString username, int siteId, QByteArray image, QList<int> &documentUris): username(username), siteId(siteId), image(image)
{
    this->documentUris = documentUris;
}

Account::Account(const Account &other){
    this->siteId = other.siteId;
    this->username = other.username;
    this->image = other.image;
}

QString Account::toString(){
    return "username: " + this->username + " siteId: " + QString::number(this->siteId);
}
