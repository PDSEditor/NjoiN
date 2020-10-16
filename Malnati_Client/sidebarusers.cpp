#include "sidebarusers.h"
#include "ui_sidebarusers.h"

sidebarUsers::sidebarUsers(QList<QString> userOn, QList<QString> userOff, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sidebarUsers)
{
    ui->setupUi(this);

    this->onlineUsers = userOn;
    this->offlineUsers = userOff;

    for(auto user : this->onlineUsers) {
         QListWidgetItem* item = new QListWidgetItem(user);
         ui->onlineUsersUI->addItem(item);
    }

    for(auto user : this->offlineUsers) {
         QListWidgetItem* item = new QListWidgetItem(user);
         ui->offlineUsersUI->addItem(item);
    }
}

sidebarUsers::~sidebarUsers()
{
    delete ui;
}
