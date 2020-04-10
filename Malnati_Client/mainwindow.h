#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "textedit.h"
#include "accountgui.h"
#include "socketmanager.h"

extern socketManager *sock;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    AccountGui *getProfilePtr();

private slots:
    void on_pushButton_clicked();
    void addElementforUser(QString);

    void on_actionNew_triggered();

    void on_actionAccount_triggered();

    void on_actionClose_triggered();

private:
    Ui::MainWindow *ui;
    TextEdit *te;
    AccountGui *accountGui = nullptr;
    void newFile();


};
#endif // MAINWINDOW_H
