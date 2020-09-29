#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>
#include <QList>

class Account
{
private:
    QString username;
    int siteId;
    QByteArray image;
    QList<int> documentUris = {};

public:
    Account();
    Account(QString username, int siteId, QByteArray image);
    Account(QString username, int siteId, QByteArray image, QList<int> &documentUris);
    Account(const Account &other);

    QString getUsername() const;
    void setUsername(const QString &value);
    int getSiteId() const;
    void setSiteId(int value);
    QString toString();
};

#endif // ACCOUNT_H
