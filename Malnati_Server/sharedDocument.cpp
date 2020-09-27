#include "sharedDocument.h"

SharedDocument::SharedDocument()
{

}

SharedDocument::SharedDocument(QString name, int creator)
{
    this->creator = creator;
    this->userAllowed = QList<int>() << creator;
    this->open = false;
    this->name = name;
    this->symbols = QList<Symbol>();
}

QString SharedDocument::getName() const
{
    return name;
}

void SharedDocument::setName(const QString &value)
{
    name = value;
}

QList<int> SharedDocument::getUserAllowed() const
{
    return userAllowed;
}

void SharedDocument::setUserAllowed(const QList<int> &value)
{
    userAllowed = value;
}

int SharedDocument::getCreator() const
{
    return creator;
}

bool SharedDocument::getOpen() const
{
    return open;
}

void SharedDocument::setOpen(bool value)
{
    // todo: una volta chiuso, bisogna salvarlo sul disco!
    open = value;
}

QList<Symbol> SharedDocument::getSymbols() const
{
    return symbols;
}

void SharedDocument::setSymbols(const QList<Symbol> &value)
{
    symbols = value;
}


