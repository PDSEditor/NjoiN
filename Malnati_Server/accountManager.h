#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <memory>
#include <QObject>
#include<QList>
#include<QMap>
#include <account.h>

class AccountManager : public QObject
{
    Q_OBJECT
private:
    QMap<int, std::shared_ptr<Account>> onlineAccounts;         //tutti gli account online in questo momento
    QMap<QString, QList<QString>> accountsPerFile;              //tutti gli account che stanno lavorando in questo momento su un file  (documentId, lista user)
    QMap<int, Account> accounts;                                //tutti gli account

public:
    explicit AccountManager(QObject *parent = nullptr);
    bool closeDocumentByUser(QString username, QString documentId);    //togli l'account dalla lista di accountsPerFile, se è l'ultimo bisogna salvarlo



    QMap<int, std::shared_ptr<Account> > getOnlineAccounts() const;
    void setOnlineAccounts(const QMap<int, std::shared_ptr<Account> > &value);

public slots:
    void updateOnlineAccounts (int siteId);
    void removeOnlineAccounts (int siteId);

    //signals:
};

#endif // ACCOUNTMANAGER_H
