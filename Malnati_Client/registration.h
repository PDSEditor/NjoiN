#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QDialog>
#include <QAbstractButton>
#include <QMessageBox>
#include <message.h>
#include <QFileDialog>
#include <QBuffer>

namespace Ui {
class Registration;
}

class Registration : public QDialog
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr);
    ~Registration();
    void setSiteId(int s);

public slots:
    void openRw(int);
    void closeRw();

signals:
    void sendMessage(Message*);
    void sendError(QString);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::Registration *ui;
    int sId;
    QByteArray img;
};

#endif // REGISTRATION_H
