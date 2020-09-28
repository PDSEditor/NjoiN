#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>

class Account
{
    QString username;
    int siteId;
    QByteArray image;
public:
    Account();
    Account(QString username, int siteId, QByteArray image);
    Account(const Account &other);

    QString getUsername() const;
    void setUsername(const QString &value);
    int getSiteId() const;
    void setSiteId(int value);
    QString toString();
};

#endif // ACCOUNT_H
