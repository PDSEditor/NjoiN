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
    QMap<int, std::shared_ptr<Account>> onlineAccounts;         //tutti gli account online in questo momento      (siteId, Account)
    QMap<QString, QList<QString>> accountsPerFile;              //tutti gli account che stanno lavorando in questo momento su un file  (documentId, lista username)
    QMap<QString, QString> accountOnDocument;                   //mappa username e documento aperto

public:
    explicit AccountManager(QObject *parent = nullptr);
    bool closeDocumentByUser(QString username, QString documentId);    //togli l'account dalla lista di accountsPerFile, se è l'ultimo bisogna salvarlo



    QMap<int, std::shared_ptr<Account> > getOnlineAccounts() const;
    void setOnlineAccounts(const QMap<int, std::shared_ptr<Account> > &value);

    QMap<QString, QList<QString> > getAccountsPerFile() const;
    void setAccountsPerFile(const QMap<QString, QList<QString> > &value);


    bool updateOnlineAccounts (int siteId, const Account &acc);
    void updateAccountOnDocument (QString user, QString documentId);



    QMap<QString, QString> getAccountOnDocument() const;
    void setAccountOnDocument(const QMap<QString, QString> &value);

public slots:
    void removeOnlineAccounts (int siteId);

    //signals:
};

#endif // ACCOUNTMANAGER_H
