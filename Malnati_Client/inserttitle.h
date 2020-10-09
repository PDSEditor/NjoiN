#ifndef INSERTTITLE_H
#define INSERTTITLE_H

#include <QDialog>

namespace Ui {
class InsertTitle;
}


class InsertTitle : public QDialog
{
    Q_OBJECT

public:
    explicit InsertTitle(QDialog *parent = nullptr);
    ~InsertTitle();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

signals:
    void setTitle(QString& title);

    void showMw();

private:
    Ui::InsertTitle *ui;
};

#endif // INSERTTITLE_H
