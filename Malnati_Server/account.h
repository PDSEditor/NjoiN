#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>
#include <QList>
#include <QBuffer>
#include <QPixmap>
#include <QIODevice>

class Account
{
private:
    QString username;
    int siteId;
    QByteArray image;
    QList<QString> documentUris;

public:
    Account();
    Account(QString username, int siteId, QByteArray& image);
    Account(QString username, int siteId);
    Account(QString username, int siteId, QByteArray& image, QList<QString> &documentUris);
    Account(const Account &other);

    QString getUsername() const;
    void setUsername(const QString &value);
    int getSiteId() const;
    void setSiteId(int value);
    QString toString();
    QList<QString> getDocumentUris() const;
    void setDocumentUris(const QList<QString> value);
    void setImage(QPixmap Pix);
    QPixmap getImage() const;
};

#endif // ACCOUNT_H
