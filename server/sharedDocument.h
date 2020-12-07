#ifndef SHAREDDOCUMENT_H
#define SHAREDDOCUMENT_H

#include <QString>
#include <QList>
#include "symbol.h"


class SharedDocument
{
private:
    QString uri;
    QString name;
    QList<QString> userAllowed;
    QString creator;
    bool open;
    QList<Symbol> symbols;

public:
    SharedDocument();
    SharedDocument(QString name, QString creator);
    SharedDocument(QString name, QString creator, bool open, QList<QString> &userAllowed);
    QString getUri() const;
    void setUri(const QString &value);
    QString getName() const;
    void setName(const QString &value);
    QList<QString> getUserAllowed() const;
    void setUserAllowed(const QList<QString> &value);
    QString getCreator() const;
    void setCreator(const QString &value);
    bool getOpen() const;
    void setOpen(bool value);
    QList<Symbol> getSymbols() const;
    void setSymbols(const QList<Symbol> &value);

    bool operator==(SharedDocument doc);
};

#endif // SHAREDDOCUMENT_H
