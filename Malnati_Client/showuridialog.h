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
    explicit ShowUriDialog(QWidget *parent = 0);

    void setUriDialog(QString uri);
    ~ShowUriDialog();
protected:
    void closeEvent(QCloseEvent *e) override;
private:
    Ui::ShowUriDialog *ui;
    QSignalBlocker *b;
};

#endif // SHOWURIDIALOG_H
