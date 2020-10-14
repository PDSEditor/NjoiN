#ifndef INSERTTITLE_H
#define INSERTTITLE_H

#include <QDialog>

namespace Ui {
class InsertTitle;
}


class InsertTitle : public QDialog
{
    Q_OBJECT
protected:
    void closeEvent(QCloseEvent *event) override;
public:
    explicit InsertTitle(QDialog *parent = nullptr);
    explicit InsertTitle(QList<QString> s,QDialog *parent = nullptr);
    ~InsertTitle();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

signals:
    void setTitle(QString& title);

    void showMw(QString);

private:
    Ui::InsertTitle *ui;
    QList<QString> names;
};

#endif // INSERTTITLE_H
