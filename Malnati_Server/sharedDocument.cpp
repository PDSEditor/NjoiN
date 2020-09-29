#include "sharedDocument.h"

QList<QString> SharedDocument::getUserAllowed() const
{
    return userAllowed;
}

void SharedDocument::setUserAllowed(const QList<QString> &value)
{
    userAllowed = value;
}

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

SharedDocument::SharedDocument(QString name, int creator, bool open, QList<int> &userAllowed): name(name), creator(creator), open(open), userAllowed(userAllowed)
{
    this->uri = name + '_' + QString::number(creator);
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

QString SharedDocument::getUri(){
    return this->uri;
}


