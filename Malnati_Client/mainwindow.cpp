#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include "loginwindow.h"
#include "accountinterface.h"

socketManager *sock;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setStyleSheet("QLabel { background-color : green; color : black; }");
    for(int i=0;i<10;i++){
        addElementforUser("Documento " + QString::number(i));
    }
//sock=new socketManager(QUrl(QStringLiteral("ws://localhost:1234")));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::newFile(){
    te = new TextEdit(this);

    const QRect availableGeometry = QApplication::desktop()->availableGeometry(te);
    te->resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    te->move((availableGeometry.width() - te->width()) / 2,
           (availableGeometry.height() - te->height()) / 2);

    //APRE UNA PAGINA DI PRESENTAZIONE DEL TEXTEDIT
   // if (!mw.load(parser.positionalArguments().value(0, QLatin1String(":/example.html"))))

    te->fileNew();
    emit(newTextEdit(te));
    te->show();
    // This is available in all editors.

}

void MainWindow::on_pushButton_clicked()
{
    newFile();

}

void MainWindow::addElementforUser(QString string){
    ui->listWidget->addItem(string);
}

void MainWindow::on_actionNew_triggered()
{
    newFile();
}

void MainWindow::on_actionAccount_triggered()
{
    AccountInterface ai;
    ai.exec();

}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}
