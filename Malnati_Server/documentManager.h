#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QObject>
#include <QBuffer>
#include <QFile>
#include "sharedDocument.h"

class DocumentManager : public QObject
{
    Q_OBJECT
public:
    explicit DocumentManager(QObject *parent = nullptr);
    bool saveToServer(SharedDocument& document);

signals:

};

#endif // DOCUMENTMANAGER_H
