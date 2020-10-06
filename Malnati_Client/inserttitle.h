#ifndef INSERTTITLE_H
#define INSERTTITLE_H

#include <QWidget>

namespace Ui {
class InsertTitle;
}

class InsertTitle : public QWidget
{
    Q_OBJECT

public:
    explicit InsertTitle(QWidget *parent = nullptr);
    ~InsertTitle();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

signals:
    void setTitle(QString& title);

private:
    Ui::InsertTitle *ui;
};

#endif // INSERTTITLE_H
