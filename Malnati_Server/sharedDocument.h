#ifndef SHAREDDOCUMENT_H
#define SHAREDDOCUMENT_H

#include <QString>
#include <QList>
#include "symbol.h"


class SharedDocument
{
    QString uri;
    QString name;
    QList<QString> userAllowed;
    int creator;
    bool open;
    QList<Symbol> symbols;

public:
    SharedDocument();
    SharedDocument(QString name, int creator);
    SharedDocument(QString name, int creator, bool open, QList<int> &userAllowed);
    QString getName() const;
    void setName(const QString &value);

    int getCreator() const;
    bool getOpen() const;
    void setOpen(bool value);
    QList<Symbol> getSymbols() const;
    void setSymbols(const QList<Symbol> &value);
    QString getUri();
    QList<QString> getUserAllowed() const;
    void setUserAllowed(const QList<QString> &value);
};

#endif // SHAREDDOCUMENT_H
