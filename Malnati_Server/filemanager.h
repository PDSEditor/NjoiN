#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QFile>
#include <QVector>
#include <QBuffer>
#include "sharedDocument.h"

class FileManager
{
//    QVector<QString> files;

public:
    FileManager();
    bool saveToServer(SharedDocument& document);
};

#endif // FILEMANAGER_H
