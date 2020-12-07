#include "sharedDocument.h"

QString SharedDocument::getUri() const
{
    if(uri != nullptr || this->uri.size()>0)
        return uri;
    else return nullptr;
}

void SharedDocument::setUri(const QString &value)
{
    uri = value;
}

QString SharedDocument::getName() const
{
    return name;
}

void SharedDocument::setName(const QString &value)
{
    name = value;
}

QList<QString> SharedDocument::getUserAllowed() const
{
    return userAllowed;
}

void SharedDocument::setUserAllowed(const QList<QString> &value)
{
    userAllowed = value;
}

QString SharedDocument::getCreator() const
{
    return creator;
}

void SharedDocument::setCreator(const QString &value)
{
    creator = value;
}

bool SharedDocument::getOpen() const
{
    return open;
}

void SharedDocument::setOpen(bool value)
{
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

bool SharedDocument::operator==(SharedDocument doc)
{
    return this->uri == doc.uri;
}

SharedDocument::SharedDocument()
{
    
}

SharedDocument::SharedDocument(QString name, QString creator)
{
    this->creator = creator;
    this->userAllowed = QList<QString>() << creator;
    this->open = false;
    this->name = name;
    this->symbols = QList<Symbol>();
    this->uri = name + '_' + creator;
}

SharedDocument::SharedDocument(QString name, QString creator, bool open, QList<QString> &userAllowed): name(name), creator(creator), open(open), userAllowed(userAllowed)
{
    this->uri = name + '_' + creator;
}


