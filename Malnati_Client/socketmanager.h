#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>

class socketManager : public QObject
{
    Q_OBJECT
public:
    explicit socketManager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SOCKETMANAGER_H
