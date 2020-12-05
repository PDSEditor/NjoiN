#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QObject>
#include <QBuffer>
#include <QFile>
#include "sharedDocument.h"
#include "qdebug.h"

class DocumentManager : public QObject
{
    Q_OBJECT

private:
    QList<SharedDocument> openDocuments;

public:
    explicit DocumentManager(QObject *parent = nullptr);
    bool saveToServer(QString documentId, QList<Symbol> &symbols);
    bool checkPermission(QString username, QString documentId);       //controlla se l'account può accedere al file
    void openDocument(SharedDocument &doc);
    void closeDocument(QString uri);

signals:

};

#endif // DOCUMENTMANAGER_H
