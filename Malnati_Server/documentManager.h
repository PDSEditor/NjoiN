#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QObject>

class documentManager : public QObject
{
    Q_OBJECT
public:
    explicit documentManager(QObject *parent = nullptr);

signals:

};

#endif // DOCUMENTMANAGER_H
