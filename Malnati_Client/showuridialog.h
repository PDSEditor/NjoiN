#ifndef SHOWURIDIALOG_H
#define SHOWURIDIALOG_H

#include <QDialog>

namespace Ui {
class ShowUriDialog;
}

class ShowUriDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowUriDialog(QWidget *parent = nullptr);
    explicit ShowUriDialog(QWidget *parent = nullptr,QString uri=nullptr);
    ~ShowUriDialog();

private:
    Ui::ShowUriDialog *ui;
};

#endif // SHOWURIDIALOG_H
