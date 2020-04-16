#ifndef CHANGEPWD_H
#define CHANGEPWD_H

#include <QDialog>

namespace Ui {
    class ChangePwd;
}

class ChangePwd : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePwd(QWidget *parent = nullptr);
    ~ChangePwd();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

signals:
    void pwdUpdate(QString& pwd);

private:
    Ui::ChangePwd *ui = nullptr;
};

#endif // CHANGEPWD_H
