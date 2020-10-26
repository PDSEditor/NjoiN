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
         QList<QListWidgetItem*> q=ui->onlineUsersUI->findItems(item->text(),Qt::MatchExactly);
         if(q.size()==0)
            ui->onlineUsersUI->addItem(item);
    }

    for(auto user : this->offlineUsers) {
         QListWidgetItem* item = new QListWidgetItem(user);
         QList<QListWidgetItem*> q=ui->offlineUsersUI->findItems(item->text(),Qt::MatchExactly);
         if(q.size()==0)
            ui->offlineUsersUI->addItem(item);
    }
}

sidebarUsers::~sidebarUsers()
{
    delete ui;
}
