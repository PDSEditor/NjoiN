#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QObject>
#include<QList>
#include<QMap>
#include <account.h>

class AccountManager : public QObject
{
    Q_OBJECT
public:
    explicit AccountManager(QObject *parent = nullptr);
    void checkUserPerFile(int siteId, QString);

signals:

public slots:
    void updateOnlineAccounts (int siteId);
    void removeOnlineAccounts (int siteId);
private:

    QMap<int, Account *> onlineAccounts;
    QMap<QString, QList<int>> accountsPerFile;
    QMap<int, Account> accounts;


};

#endif // ACCOUNTMANAGER_H
