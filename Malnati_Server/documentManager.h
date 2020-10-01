#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QObject>
#include <QBuffer>
#include <QFile>
#include "sharedDocument.h"

class DocumentManager : public QObject
{
    Q_OBJECT

private:
    QList<SharedDocument> openDocuments;

public:
    explicit DocumentManager(QObject *parent = nullptr);
    bool saveToServer(SharedDocument& document);
    void checkPermission(QString username, QString documentId);       //controlla se l'account può accedere al file

signals:

};

#endif // DOCUMENTMANAGER_H
