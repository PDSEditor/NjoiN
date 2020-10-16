#ifndef SIDEBARUSERS_H
#define SIDEBARUSERS_H

#include <QWidget>

namespace Ui {
class sidebarUsers;
}

class sidebarUsers : public QWidget
{
    Q_OBJECT

public:
    explicit sidebarUsers(QList<QString> userOn, QList<QString> userOff, QWidget *parent = nullptr);
    ~sidebarUsers();
    void addOnlineUsers(QString user);


private:
    Ui::sidebarUsers *ui;

    QList<QString> onlineUsers;
    QList<QString> offlineUsers;

};

#endif // SIDEBARUSERS_H
