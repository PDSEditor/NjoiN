#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QObject>
#include <sharedDocument.h>

class documentManager : public QObject
{
    Q_OBJECT

private:
    QList<SharedDocument>

public:
    explicit documentManager(QObject *parent = nullptr);
    void checkPermission(QString username, QString documentId);       //controlla se l'account può accedere al file



signals:

};

#endif // DOCUMENTMANAGER_H
