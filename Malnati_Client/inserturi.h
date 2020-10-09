#ifndef INSERTURI_H
#define INSERTURI_H

#include <QAbstractButton>
#include <QDialog>
#include "message.h"

namespace Ui {
class Inserturi;
}

class Inserturi : public QDialog
{
    Q_OBJECT

public:
    explicit Inserturi(QWidget *parent = nullptr);
    ~Inserturi();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_buttonBox_rejected();

signals:
    void sendUri(Message);

private:
    Ui::Inserturi *ui;
};

#endif // INSERTURI_H
