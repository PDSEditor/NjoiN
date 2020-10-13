#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QDialog>
#include <QAbstractButton>
#include<QMessageBox>
#include <message.h>

namespace Ui {
class Registration;
}

class Registration : public QDialog
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr);
    ~Registration();
    void setiteId(int s);

public slots:
    void receiveRegistration(Message);
     void openRw(int);
     void closeRw();
signals:
    void sendMessage(Message*);
    void sendError(QString);
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Registration *ui;
    int sId;
};

#endif // REGISTRATION_H
