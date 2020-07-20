#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QObject>

class AccountManager : public QObject
{
    Q_OBJECT
public:
    explicit AccountManager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // ACCOUNTMANAGER_H
