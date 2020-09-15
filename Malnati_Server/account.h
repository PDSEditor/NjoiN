#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>

class Account
{
    QString username;
    int siteId;
public:
    Account();
    Account(QString username, int siteId);

    QString getUsername() const;
    void setUsername(const QString &value);
    int getSiteId() const;
    void setSiteId(int value);
    QString toString();
};

#endif // ACCOUNT_H
