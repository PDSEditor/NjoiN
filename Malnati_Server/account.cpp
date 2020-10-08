#include "account.h"

Account::Account(){
    //this->username = "Placeholder";                     // per riconoscere un account creato solo temporaneamente
}

Account::Account(QString username, int siteId, QByteArray& image, QList<QString> &documentUris): username(username), siteId(siteId), image(image)
{
    this->documentUris = documentUris;
}

Account::Account(QString username, int siteId, QByteArray& image): username(username), siteId(siteId), image(image)
{}


QList<QString> Account::getDocumentUris() const
{
    return documentUris;
}

void Account::setDocumentUris(const QList<QString> value)
{
    documentUris = value;
}

void Account::setImage(QPixmap Pix)
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    Pix.save(&buffer, "PNG");
    auto const encoded = buffer.data().toBase64();
    this->image = QLatin1String(encoded).latin1();
}

QPixmap Account::getImage() const {
    QPixmap p;
    p.loadFromData(QByteArray::fromBase64(image), "PNG");
    return p;
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

Account::Account(const Account &other){
    this->siteId = other.siteId;
    this->username = other.username;
    this->image = other.image;
}

QString Account::toString(){
    return "username: " + this->username + " siteId: " + QString::number(this->siteId);
}
